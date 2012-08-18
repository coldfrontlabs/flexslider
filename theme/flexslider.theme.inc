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
  
}

/**
 * Default theme implementation for flexslider_list_item
 */
function theme_flexslider_list_item(&$vars) {
  
}

/**
 * Template preprocess handler for 'flexslider_container' theme.
 */
function template_preprocess_flexslider(&$vars) {
  // Each flexslider instance gets a unique id
  $flexslider_id = &drupal_static('flexslider_id', 0);
  $vars['id'] = ++$flexslider_id;

  // Load the used option set
  if (!empty($vars['settings']['flexslider_optionset'])) {
    $optionset = flexslider_optionset_load($vars['settings']['flexslider_optionset']);
  }
  if (empty($optionset)) {
    // Fall back to 'default' option set
    $optionset = flexslider_optionset_load('default');
  }
  
  // Attach flexslider JavaScript
  flexslider_add($flexslider_id, $optionset);

  // Prepare image elements
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