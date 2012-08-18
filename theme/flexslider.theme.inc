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
  $optionset = &$vars['settings']['optionset'];
  $settings = &$vars['settings'];
  $items = &$vars['items'];

  // Set the list type
  // @todo make this configurable
  $settings['type'] = 'ul';

  // Load the selected optionset
  if (!empty($optionset)) {
    $optionset = flexslider_optionset_load($optionset);
  }
  
  // Check if an optionset was loaded
  if (empty($optionset)) {
    // Fall back to 'default' option set
    $optionset = flexslider_optionset_load('default');
    watchdog('flexslider', 'Fallback to default optionset.', array(), WATCHDOG_WARNING);
  }

  // Configure attributes for containing elements
  $attributes = array();
  // Merge with defined attributes
  if (isset($settings['attributes']) and is_array($settings['attributes'])) {
    $attributes += $settings['attributes'];
  }

  // Each flexslider instance gets a unique id
  $flexslider_id = &drupal_static('flexslider_id', 0);
  $attributes['id'] = 'flexslider-' . ++$flexslider_id;
  // Add default class
  $attributes['class'][] = 'flexslider';
  
  // Add the namespace to any classes
  if (!empty($optionset->options['namespace'])) {
    foreach ($attributes['class'] as $key => $value) {
      $attributes['class'][$key] = $optionset->options['namespace'] . $value;
    }
  }

  // Add the attributes to the settings array.
  $settings['attributes'] = $attributes;

  // Prepare slide elements
  // @todo detect the image style here or leave it to the list_item template?
  // @todo make this non-image specific
  /*$items = $vars['items'];
  $vars['items'] = array();
  $thumb_style = empty($optionset->imagestyle_thumb) ? 'flexslider_thumb' : $optionset->imagestyle_thumb;
  foreach ($items as $delta => $item) {
    $vars['items'][$delta] = array(
      '#theme' => 'image_formatter',
      '#item' => $item,
      '#image_style' => $optionset->imagestyle_normal,
    );
  }*/
  
  // Finally, add the FlexSlider library
  flexslider_add($settings['attributes']['id'], $optionset);
}


/**
 * Preprocess function for flexslider_list_item
 */
function template_preprocess_flexslider_list(&$vars) {
  // @todo
}

/**
 * Preprocess function for flexslider_list_item
 */
function template_preprocess_flexslider_list_item(&$vars) {
  // @todo
}