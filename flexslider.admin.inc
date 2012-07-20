<?php
/**
 * @file
 * Administrative page callbacks for the flexslider module.
 */


/**
 * Menu callback; Listing of all current option sets.
 */
function flexslider_page_optionset_list() {
  $optionsets = flexslider_optionset_load_all();
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

  $table_element = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('There are currently no option sets. <a href="!url">Add a new one</a>.', array('!url' => url('admin/config/media/flexslider/add'))),
  );

  return drupal_render($table_element);
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
    '#default_value' => isset($options['animation']) ? $options['animation'] : _flexslider_optionset_defaults('animation'),
    // @todo add states to enable/disable the direction
  );
  
  $form['animation_slideshow']['animationSpeed'] = array(
    '#type' => 'textfield',
    '#title' => t('Animation Speed'),
    '#description' => t('Set the speed of animations, in milliseconds'),
    '#element_validate' => array('_flexslider_validate_positive_integer'),
    '#default_value' => isset($options['animationSpeed']) ? $options['animationSpeed'] : _flexslider_optionset_defaults('animationSpeed'),
    '#size' => 30,
  );
  
  $form['animation_slideshow']['direction'] = array(
    '#type' => 'select',
    '#title' => t('Slide Direction'),
    '#description' => t('Select the sliding direction, "horizontal" or "vertical"'),
    '#options' => array(
      'horizontal'   => t('Horizontal'),
      'vertical'  => t('Vertical'),
    ),
    '#default_value' =>  isset($options['direction']) ? $options['direction'] : _flexslider_optionset_defaults('direction'),
  );

  $form['animation_slideshow']['slideshow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Slideshow'),
    '#description' => t('Animate the slides automatically'),
    '#default_value' => isset($options['slideshow']) ? $options['slideshow'] : _flexslider_optionset_defaults('slideshow'),
  );

  $form['animation_slideshow']['easing'] = array(
    '#type' => 'select',
    '#title' => t('Easing'),
    '#multiple' => FALSE,
    '#description' => t('The description appears usually below the item.'),
    '#options' => array(
      'linear' => t('Linear'),
      'swing' => t('Swing'),
    ), // @todo load list of options based on jquery easing plugin
    '#default_value' => isset($options['easing']) ? $options['easing'] : _flexslider_optionset_defaults('easing'),
  );
  
  $form['animation_slideshow']['reverse'] = array(
    '#type' => 'checkbox',
    '#title' => t('Reverse'),
    '#description' => t('Animate the slides in reverse'),
    '#default_value' => isset($options['reverse']) ? $options['reverse'] : _flexslider_optionset_defaults('reverse'),
  );

  $form['animation_slideshow']['slideshowSpeed'] = array(
    '#type' => 'textfield',
    '#title' => t('Slideshow speed'),
    '#description' => t('Set the speed of the slideshow cycling, in milliseconds'),
    '#element_validate' => array('_flexslider_validate_positive_integer'),
    '#default_value' => isset($options['slideshowSpeed']) ? $options['slideshowSpeed'] : _flexslider_optionset_defaults('slideshowSpeed'),
    '#size' => 30,
  );

  $form['animation_slideshow']['animationLoop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Loop Slideshow'),
    '#description' => t('Loop the slideshow once it reaches the last slide.'),
    '#default_value' => isset($options['animationLoop']) ? $options['animationLoop'] : _flexslider_optionset_defaults('animationLoop'),
  );

  $form['animation_slideshow']['randomize'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize Slide Order'),
    '#description' => t('Randomize the order the slides play back.'),
    '#default_value' => isset($options['randomize']) ? $options['randomize'] : _flexslider_optionset_defaults('randomize'),
  );
  $form['animation_slideshow']['startAt'] = array(
    '#type' => 'textfield',
    '#title' => t('Starting Slide'),
    '#description' => t('The slide that the slider should start on. Ex: For the first slide enter "0", for the second enter "1", etc. If you enter a value which is greater than the number of slides, the slider will default to the first slide.'),
    '#element_validate' => array('_flexslider_validate_positive_integer'),
    '#default_value' => isset($options['startAt']) ? $options['startAt'] : _flexslider_optionset_defaults('startAt'),
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
    '#default_value' => isset($options['directionNav']) ? $options['directionNav'] : _flexslider_optionset_defaults('directionNav'),
  );
  $form['nav_controls']['controlNav'] = array(
    '#type' => 'checkbox',
    '#title' => t('Paging Controls'),
    '#description' => t('Add controls to jump to individual slides.'),
    '#default_value' => isset($options['controlNav']) ? $options['controlNav'] : _flexslider_optionset_defaults('controlNav'),
    // @todo add states options for manualControls
  );
  $form['nav_controls']['keyboard'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keyboard Navigation'),
    '#description' => t('Allow slider navigating via keyboard left/right keys'),
    '#default_value' => isset($options['keyboard']) ? $options['keyboard'] : _flexslider_optionset_defaults('keyboard'),
  );
  $form['nav_controls']['mousewheel'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mousewheel Navigation'),
    '#description' => t('Allow slider navigating via mousewheel'),
    '#default_value' => isset($options['mousewheel']) ? $options['mousewheel'] : _flexslider_optionset_defaults('mousewheel'),
    // @todo add check for jquery mousewheel library
  );
  $form['nav_controls']['prevText'] = array(
    '#type' => 'textfield',
    '#title' => t('Previous Link Text'),
    '#description' => t('Set the text for the "previous" control item. <em>Text translation can be controlled using the <a href="http://drupal.org/project/stringoverrides">String Overrides module</a>.</em>'),
    '#default_value' => isset($options['prevText']) ? $options['prevText'] : _flexslider_optionset_defaults('prevText'),
  );
  $form['nav_controls']['nextText'] = array(
    '#type' => 'textfield',
    '#title' => t('Next Link Text'),
    '#description' => t('Set the text for the "next" control item. <em>Text translation can be controlled using the <a href="http://drupal.org/project/stringoverrides">String Overrides module</a>.</em>'),
    '#default_value' => isset($options['nextText']) ? $options['nextText'] : _flexslider_optionset_defaults('nextText'),
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
    '#default_value' => isset($options['pausePlay']) ? $options['pausePlay'] : _flexslider_optionset_defaults('pausePlay'),
    // @todo add states value for pause/play text
  );
  $form['advanced']['pauseText'] = array(
    '#type' => 'textfield',
    '#title' => t('Pause State Text'),
    '#description' => t('Set the text for the "pause" state indicator. <em>Text translation can be controlled using the <a href="http://drupal.org/project/stringoverrides">String Overrides module</a>.</em>'),
    '#default_value' => isset($options['pauseText']) ? $options['pauseText'] : _flexslider_optionset_defaults('pauseText'),
  );
  $form['advanced']['playText'] = array(
    '#type' => 'textfield',
    '#title' => t('Play State Text'),
    '#description' => t('Set the text for the "play" state indicator. <em>Text translation can be controlled using the <a href="http://drupal.org/project/stringoverrides">String Overrides module</a>.</em>'),
    '#default_value' => isset($options['playText']) ? $options['playText'] : _flexslider_optionset_defaults('playText'),
  );
  $form['advanced']['pauseOnAction'] = array(
    '#type' => 'checkbox',
    '#title' => t('Pause On Controls'),
    '#description' => t('Pause the slideshow when interacting with control elements.'),
    '#default_value' => isset($options['pauseOnAction']) ? $options['pauseOnAction'] : _flexslider_optionset_defaults('pauseOnAction'),
  );
  $form['advanced']['controlsContainer'] = array(
    '#type' => 'textfield',
    '#title' => t('Controls container (Advanced)'),
    '#description' => t('Declare which container the navigation elements should be appended too. Default container is the flexSlider element. Example use would be ".flexslider-container", "#container", etc. If the given element is not found, the default action will be taken.'),
    '#default_value' => isset($options['controlsContainer']) ? $options['controlsContainer'] : _flexslider_optionset_defaults('controlsContainer'),
  );
  $form['advanced']['manualControls'] = array(
    '#type' => 'textfield',
    '#title' => t('Manual controls (Advanced)'),
    '#description' => t('Declare custom control navigation. Example would be ".flex-control-nav li" or "#tabs-nav li img", etc. The number of elements in your controlNav should match the number of slides/tabs.'),
    '#default_value' => isset($options['manualControls']) ? $options['manualControls'] : _flexslider_optionset_defaults('manualControls'),
  );
  
  return $form;
}

/**
 * Form builder; Form to edit a given option set.
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

  $default_options = flexslider_option_elements($optionset->options);
  // Add the options to the vertical tabs section
  foreach($default_options as $key => $value) {
    $form['options'][] = $value;
  }
  
  // Add form actions
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#name' => 'submit',
      '#value' => t('Save Option Set')
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
  $optionset->imagestyle_normal = $form_state['values']['image_style']['normal'];

  // Assign the values to the option set
  $optionset->options = _flexslider_optionset_defaults();

  // Save all the values for the optionset
  foreach($optionset->options as $key => $value) {
    if (array_key_exists($key, $form_state['values'])) {
      $optionset->options[$key] = $form_state['values'][$key];
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
    t('Are you sure you want to @action the option set %name?', array('@action' => t(drupal_strtolower($op)), '%name' => $optionset->name)),
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
 * Submit handler for the advanced module settings.
 */
function flexslider_form_settings_submit($form, &$form_state) {
  // Do nothing for now
}

/**
 * Export option sets
 */
function flexslider_form_optionset_export() {
  $output = '';
  
  // Check for an appropriate export tool
  if (module_exists('bulk_export') || module_exists('features')) {
    $output .= 'Use any of the following to export your option sets.';
    $output .= '<ul>';
    if (module_exists('bulk_export')) {
      $output .= '<li>' . l('Bulk Export', 'admin/structure/bulk-export') . '</li>';
    }
    if (module_exists('features')) {
      $output .= '<li>' . l('Features', 'admin/structure/features') . '</li>';
    }
    $output .= '</ul>';
  }
  // Advise the user on tools available to them
  else {
    $output .= t('Enable the <a href="http://drupal.org/project/ctools">Bulk Export module</a> or the <a href="http://drupal.org/project/features">Features module</a> to export your option sets.');
  }

  return $output;
}

/**
 * Import option set form
 */
function flexslider_form_optionset_import() {
  $output = '<p>' . t('At present, the only method of importing option sets is by including the preset in a module. The Features module can simplify this process.') . '</p>';
  $output .= '<p>' . t('A form based import/export mechanism will be added in the future.') . '</p>';
  return $output;
}
