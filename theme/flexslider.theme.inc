<?php
/**
 * @file
 * Theming functions for the flexslider module.
 * 
 * Preprocessor functions fill variables for templates and helper
 * functions to make theming easier.
 */

/**
 * Default theme implementation for flexslider_list
 */
function theme_flexslider_list(&$vars) {
  // Reference configuration variables
  $optionset = &$vars['settings']['optionset'];
  $items = &$vars['items'];
  dpm('flex list');
  dpm($vars);
  
  $type = &$vars['settings']['type'];
  //$attributes = $vars['attributes'];
  $attributes = array();

  $output = '';

  if (!empty($items)) {
    $output .= "<$type" . drupal_attributes($attributes) . '>';
    foreach ($items as $i => $item) {
      $output .= theme('flexslider_list_item', array('item' => $item, 'optionset' => $optionset));
    }
    $output .= "</$type>";
  }
  return $output;
}

/**
 * Default theme implementation for flexslider_list_item
 */
function theme_flexslider_list_item(&$vars) {
  dpm('flex list item');
  dpm($vars);
  $attributes = array();
  $data = '';
  // @todo
  $output = '';
  $output .= '<li' . drupal_attributes($attributes) . '>' . $data . "</li>\n";
  
  return $output;
}

/**
 * Template preprocess handler for 'flexslider' theme.
 */
function template_preprocess_flexslider(&$vars) {
  dpm('flexslider');
  dpm($vars);
  // Each flexslider instance gets a unique id
  $flexslider_id = &drupal_static('flexslider_id', 0);
  $vars['id'] = ++$flexslider_id;

  // Set the list type
  // @todo make this configurable
  $vars['settings']['type'] = 'ul';

  // Load the used option set
  if (!empty($vars['settings']['optionset'])) {
    $optionset = flexslider_optionset_load($vars['settings']['optionset']);
  }
  if (empty($optionset)) {
    // Fall back to 'default' option set
    $optionset = flexslider_optionset_load('default');
  }
  
  // Assign the optionset values to the settings array
  $vars['settings']['optionset'] = $optionset;
  
  // Attach flexslider JavaScript
  flexslider_add($flexslider_id, $optionset);

  // Prepare slide elements
  // @todo detect the image style here or leave it to the list_item template?
  // @todo make this non-image specific 
  $items = $vars['items'];
  $vars['items'] = array();
  $thumb_style = empty($optionset->imagestyle_thumb) ? 'flexslider_thumb' : $optionset->imagestyle_thumb;
  foreach ($items as $delta => $item) {
    $vars['items'][$delta] = array(
      '#theme' => 'image_formatter',
      '#item' => $item,
      '#image_style' => $optionset->imagestyle_normal,
    );
  }
}