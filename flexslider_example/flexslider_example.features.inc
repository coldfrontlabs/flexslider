<?php
/**
 * @file
 * flexslider_example.features.inc
 */

/**
 * Implements hook_views_api().
 */
function flexslider_example_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function flexslider_example_node_info() {
  $items = array(
    'flexslider_example' => array(
      'name' => t('FlexSlider Example'),
      'base' => 'node_content',
      'description' => t('Used for generating example FlexSlider implementations'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
