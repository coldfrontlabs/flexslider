<?php
/**
 * @file
 * Administrative page callbacks for the flexslider module.
 */


/**
 * Menu callback; Listing of all current option sets.
 */
function flexslider_page_optionset_list() {
  ctools_include('export');
  $optionsets = ctools_export_crud_load_all('flexslider_optionset');
  $header = array(t('Option Set Name'), array('data' => t('Operations'), 'colspan' => 2));
  $rows = array();
  foreach ($optionsets as $name => $optionset) {
    $is_overridden = ($optionset->export_type & EXPORT_IN_CODE) ? TRUE : FALSE;

    $rows[] = array(
      l($optionset->title, 'admin/config/media/flexslider/edit/' . $name),
      l(t('edit'), 'admin/config/media/flexslider/edit/' . $name),
      // Only print revert/delete links if we have something in the db to delete
      (!isset($optionset->in_code_only)) ? l(t($is_overridden ? 'revert' : 'delete'), 'admin/config/media/flexslider/delete/' . $name) : '',
    );
  }

  return theme('table', array(
      'header' => $header,
      'rows' => $rows,
      'empty' => t('There are currently no option sets. <a href="!url">Add a new one</a>.', array('!url' => url('admin/config/media/flexslider/add'))),
  ));
}


/**
 * Form builder; Form for adding a new option set.
 */
function flexslider_form_optionset_add($form, &$form_state) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#maxlength' => '255',
    '#title' => t('Title'),
    '#description' => t('A human-readable title for this option set.'),
    '#required' => TRUE,
  );
  $form['name'] = array(
    '#type' => 'machine_name',
    '#maxlength' => '255',
    '#machine_name' => array(
      'source' => array('title'),
      'exists' => 'flexslider_optionset_exists',
    ),
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Create new option set'),
    ),
    'cancel' => array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => 'admin/config/media/flexslider',
    )
  );

  return $form;
}

/**
 * Submit handler for adding a new option set.
 */
function flexslider_form_optionset_add_submit($form, &$form_state) {
  ctools_include('export');

  $optionset = ctools_export_crud_new('flexslider_optionset');

  $optionset->name = $form_state['values']['name'];
  $optionset->title = $form_state['values']['title'];
  $optionset->options = array();
    
  $saved = ctools_export_crud_save('flexslider_optionset', $optionset);

  drupal_set_message(t('Option set %name was created.', array('%name' => $optionset->name)));
  $form_state['redirect'] = 'admin/config/media/flexslider/edit/' . $optionset->name;
}

/**
 * This function returns an array defining the form elements used to edit the different options.
 *
 * @param array $options [optional]
 *  Pass in a set of default values for the options
 * @return array
 *  Returns the option set array
 */
function flexslider_option_elements($options = array()) {
  $form = array();
  
  // General Slideshow and Animiation Settings
  $form['animation_slideshow'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Slideshow and Animation Settings'),
  );
  
  $form['animation_slideshow']['animation'] = array(
    '#type' => 'select',
    '#title' => t('Animation'),
    '#description' => t('Select your animation type'),
    '#options' => array(
      'fade'   => t('Fade'),
      'slide'  => t('Slide'),
    ),
    '#default_value' => $options['animation'] ? $options['animation'] : 'fade',
    // @todo add states to enable/disable the direction
  );
  
  $form['animation_slideshow']['animationDuration'] = array(
    '#type' => 'textfield',
    '#title' => t('Animation speed'),
    '#description' => t('Set the speed of animations, in milliseconds'),
    '#element_validate' => array('_flexslider_validate_positive_integer'),
    '#default_value' => 600,
    '#size' => 30,
  );
  
  $form['animation_slideshow']['slideDirection'] = array(
    '#type' => 'select',
    '#title' => t('Slide Direction'),
    '#description' => t('Select the sliding direction, "horizontal" or "vertical"'),
    '#options' => array(
      'horizontal'   => t('Horizontal'),
      'vertical'  => t('Vertical'),
    ),
    '#default_value' => 'horizontal',
  );

  $form['animation_slideshow']['slideshow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Slideshow Playback'),
    '#description' => t('Animate the slides automatically'),
    '#default_value' => TRUE,
  );
  
  $form['animation_slideshow']['slideshowSpeed'] = array(
    '#type' => 'textfield',
    '#title' => t('Slideshow speed'),
    '#description' => t('Set the speed of the slideshow cycling, in milliseconds'),
    '#element_validate' => array('_flexslider_validate_positive_integer'),
    '#default_value' => 7000,
    '#size' => 30,
  );

  $form['animation_slideshow']['animationLoop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Loop Slideshow'),
    '#description' => t('Loop the slideshow once it reaches the last slide.'),
    '#default_value' => TRUE,
  );

  $form['animation_slideshow']['randomize'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize Slide Order'),
    '#description' => t('Randomize the order the slides play back.'),
    '#default_value' => FALSE,
  );
  $form['animation_slideshow']['slideToStart'] = array(
    '#type' => 'textfield',
    '#title' => t('Starting Slide'),
    '#description' => t('The slide that the slider should start on. Ex: For the first slide enter "0", for the second enter "1", etc. If you enter a value which is greater than the number of slides, the slider will default to the first slide.'),
    '#element_validate' => array('_flexslider_validate_positive_integer'),
    '#default_value' => 0,
    '#size' => 30,
    // @todo add states to disable if randomize is set
  );
  
  // Navigation and Control Settings
  $form['nav_controls'] = array(
    '#type' => 'fieldset',
    '#title' => t('Navigation and Control Settings'),
  );
  $form['nav_controls']['directionNav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Next/Previous Controls'),
    '#description' => t('Add controls for previous/next navigation'),
    '#default_value' => TRUE,
  );
  $form['nav_controls']['controlNav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Paging Controls'),
    '#description' => t('Add controls to jump to individual slides.'),
    '#default_value' => TRUE,
    // @todo add states options for manualControls
  );
  $form['nav_controls']['keyboardNav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keyboard Navigation'),
    '#description' => t('Allow slider navigating via keyboard left/right keys'),
    '#default_value' => TRUE,
  );
  $form['nav_controls']['mousewheel'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mousewheel Navigation'),
    '#description' => t('Allow slider navigating via mousewheel'),
    '#default_value' => TRUE,
  );
  $form['nav_controls']['prevText'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous Link Text'),
    '#description' => t('Set the text for the "previous" control item. <em>Text translation can be controlled using the <a href="http://drupal.org/project/stringoverrides">String Overrides module</a>.</em>'),
    '#default_value' => 'Previous',
  );
  $form['nav_controls']['nextText'] = array(
    '#type' => 'textfield',
    '#title' => t('Next Link Text'),
    '#description' => t('Set the text for the "next" control item. <em>Text translation can be controlled using the <a href="http://drupal.org/project/stringoverrides">String Overrides module</a>.</em>'),
    '#default_value' => 'Next',
  );

  // Advanced Options
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Options'),
  );
  $form['advanced']['pausePlay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Pause/Play Indicator'),
    '#description' => t('Have FlexSlider add an element indicating the current state of the slideshow (i.e. "pause" or "play").'),
    '#default_value' => FALSE,
    // @todo add states value for pause/play text
  );
  $form['advanced']['pauseText'] = array(
    '#type' => 'textfield',
    '#title' => t('Pause State Text'),
    '#description' => t('Set the text for the "pause" state indicator. <em>Text translation can be controlled using the <a href="http://drupal.org/project/stringoverrides">String Overrides module</a>.</em>'),
    '#default_value' => 'Pause',
  );
  $form['advanced']['playText'] = array(
    '#type' => 'textfield',
    '#title' => t('Play State Text'),
    '#description' => t('Set the text for the "play" state indicator. <em>Text translation can be controlled using the <a href="http://drupal.org/project/stringoverrides">String Overrides module</a>.</em>'),
    '#default_value' => 'Play',
  );
  $form['advanced']['pauseOnAction'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause On Controls'),
    '#description' => t('Pause the slideshow when interacting with control elements.'),
    '#default_value' => TRUE,
  );
  $form['advanced']['controlsContainer'] = array(
    '#type' => 'textfield',
    '#title' => t('Controls container (Advanced)'),
    '#description' => t('Declare which container the navigation elements should be appended too. Default container is the flexSlider element. Example use would be ".flexslider-container", "#container", etc. If the given element is not found, the default action will be taken.'),
    '#default_value' => '',
  );
  $form['advanced']['manualControls'] = array(
    '#type' => 'textfield',
    '#title' => t('Manual controls (Advanced)'),
    '#description' => t('Declare custom control navigation. Example would be ".flex-control-nav li" or "#tabs-nav li img", etc. The number of elements in your controlNav should match the number of slides/tabs.'),
    '#default_value' => '',
  );
  
  return $form;
}

/**
 * Form  builder; Form to edit a given option set.
 */
function flexslider_form_optionset_edit($form, &$form_state, $optionset) { 

  if (empty($form_state['optionset'])) {
    $form_state['optionset'] = $optionset;
  }
  else {
    $optionset = $form_state['optionset'];
  }

  // Title
  $form['title'] = array(
    '#type' => 'textfield',
    '#maxlength' => '255',
    '#title' => t('Title'),
    '#default_value' => $optionset->title,
    '#description' => t('A human-readable title for this option set.'),
    '#required' => TRUE,
  );

// @todo decide if we should remove the themes controls entirely
// seems like something that should be controlled via CSS in the theme
// as FlexSlider doesn't have "themes" functionality
/*  // Show the theme select box if there is more than one theme
  $themes = array_keys(flexslider_get_themes(TRUE));
  if (count($themes) == 1) {
    $form['theme'] = array(
      '#type' => 'hidden',
      '#value' => $themes[0],
    );
  }
  elseif (count($themes) > 1) {
    asort($themes);

    $form['theme'] = array(
      '#type' => 'select',
      '#title' => t('Theme'),
      '#description' => t('As you have more than one flexslider theme installed, please select the one to use.'),
      '#options' => array_combine($themes, $themes),
      '#default_value' => $optionset->theme,
    );
  }
*/
  // Show select boxes for the various image styles (thumbnail, normal, big)
  $image_style = image_style_options(FALSE);
  $form['image_style'] = array(
    '#type' => 'fieldset',
    '#title' => 'Image style',
    '#tree' => TRUE,
  );
  $form['image_style']['normal'] = array(
    '#type' => 'select',
    '#title' => t('Normal image style'),
    '#description' => t('Image style for the main stage images.'),
    '#empty_option' => t('None (original image)'),
    '#options' => $image_style,
    '#default_value' => $optionset->imagestyle_normal,
  );

  // Options Vertical Tab Group table
  $form['options'] = array(
    '#type' => 'vertical_tabs',
  );

  $default_options = flexslider_option_elements();
  // Add the options to the vertical tabs section
  foreach($default_options as $key => $value) {
    $form['options'][] = $value;
  }

  // Map current settings to form elements
  // @todo
  
  // Add form actions
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#name' => 'submit',
      '#value' => t('Save option set')
    ),
    'cancel' => array(
      '#type' => 'link',
      '#title' => t('Cancel'),
      '#href' => 'admin/config/media/flexslider',
    )
  );

  return $form;
}

/**
 * Validate a form element that should have an integer value.
 */
function _flexslider_validate_positive_integer($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && (!is_numeric($value) || intval($value) != $value || $value < 0)) {
    form_error($element, t('%name must be a positive integer.', array('%name' => $element['#title'])));
  }
}

/**
 * Validate a form element that should have a number as value.
 */
function _flexslider_validate_number($element, &$form_state) {
  $value = $element['#value'];
  if ($value !== '' && !is_numeric($value)) {
    form_error($element, t('%name must be a number.', array('%name' => $element['#option_name'])));
  }
}

/**
 * Submit handler for 'Save option set' button; Save the changed option set.
 */
function flexslider_form_optionset_edit_submit($form, &$form_state) {
  $optionset = &$form_state['optionset'];

  $optionset->title = $form_state['values']['title'];
  //$optionset['theme'] = $form_state['values']['theme'];
  //$optionset['imagestyle_thumb']  = $form_state['values']['image_styles']['thumb'];
  $optionset->imagestyle_normal = $form_state['values']['image_style']['normal'];
  //$optionset['imagestyle_big']    = $form_state['values']['image_styles']['big'];

  if (isset($form_state['values']['options'])) {
    foreach ($form_state['values']['options'] as $index => $values) {
      $element = $form['options'][$index]['value'];
      $value  = $values['value'];
  
      if ($value !== '') {
        // Do some typeguessing here...
        if ($element['#type'] == 'checkbox') {
          $value = (bool) $value;
        }
        elseif (is_numeric($value)) { // || intval($value) != $value || $value < 0)
          $value = (float) $value;
        }
        elseif (strcasecmp($value, 'true') == 0) {
          $value = TRUE;
        }
        elseif (strcasecmp($value, 'false') == 0) {
          $value = FALSE;
        }
      }
  
      $option = $element['#option_name'];
      $optionset->options[$option] = $value;
    }
  }

  ctools_include('export');
  ctools_export_crud_save('flexslider_optionset', $optionset);

  drupal_set_message(t('Option set %name changed.', array('%name' => $optionset->name)));
  $form_state['redirect'] = 'admin/config/media/flexslider';
}


/**
 * Form builder; Form to delete a given option set.
 */
function flexslider_optionset_form_delete($form, &$form_state, $optionset) {
  $form_state['optionset'] = &$optionset;

  // Deleting an export in code will revert it.
  $op = ($optionset->export_type & EXPORT_IN_CODE) ? 'Revert' : 'Delete';

  return confirm_form(
    $form,
    t('Are you sure you want to ' . strtolower($op) . ' the option set %name?', array('%name' => $optionset->name)),
    'admin/config/media/flexslider',
    NULL,
    t($op),  t('Cancel')
  );
}

/**
 * Submit handler for deleting an option set.
 */
function flexslider_optionset_form_delete_submit($form, &$form_state) {
  $optionset = &$form_state['optionset'];
  $op = ($optionset->export_type & EXPORT_IN_CODE) ? 'reverted' : 'deleted';

  ctools_include('export');
  ctools_export_crud_delete('flexslider_optionset', $optionset);

  drupal_set_message(t('Option set %name was ' . $op . '.', array('%name' => $optionset->name)));
  $form_state['redirect'] = 'admin/config/media/flexslider';
}


/**
 * Form builder; Form for advanced module settings.
 */
function flexslider_form_settings() {
  $form = array();

  $form['library'] = array(
    '#type' => 'fieldset',
    '#title' => 'Library',
  );

/*  $form['library']['button_clearcache'] = array(
    '#type' => 'submit',
    '#name' => 'button_clearcache',
    '#value' => t('Clear cache'),
    '#submit' => array('flexslider_form_settings_submit_clearcache'),
  );
*/
  // Debug mode toggle
  $form['library']['flexslider_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable debug mode'),
    '#description' => t('Load the human-readable version of the FlexSlider library.'),
    '#default_value' => variable_get('flexslider_debug', FALSE),
    '#access' => user_access('administer flexslider'),
  );
  
  return system_settings_form($form);
}

/**
 * Submit handler for the advanced module settings form button 'Clear cache'.
 */
function flexslider_form_settings_submit_clearcache($form, &$form_state) {
  cache_clear_all('flexslider_', 'cache', TRUE);
  drupal_set_message(t('Cache cleared.'));
}

/**
 * Submit handler for the advanced module settings.
 */
function flexslider_form_settings_submit($form, &$form_state) {
  // Do nothing for now
}
