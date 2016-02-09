<?php

/**
 * @file
 * Theme functions for FlexSlider Views.
 *
 * @author Mathew Winstone <mwinstone@coldfrontlabs.ca>
 */

/**
 * Prepares variables for flexslider view templates.
 *
 * Default template: flexslider-views-style.html.twig.
 *
 * @param array $variables
 *   An associative array containing:
 *   - view: A ViewExecutable object.
 *   - rows: The raw row data.
 *   - options: An array of options. Each option contains:
 *     - separator: A string to be placed between inline fields to keep them
 *       visually distinct.
 */
function template_preprocess_flexslider_views_style(&$variables) {
  // Only run the preprocessor if it is a view.
  if (empty($variables['view'])) {
    return;
  }

  $view = $variables['view'];
  $style = $view->style_plugin;
  $options = $style->options;

  $items = array();
  $settings = array();
  $settings['optionset'] = $options['optionset'];

  foreach ($variables['rows'] as $id => $row) {
    // Render the row into a slide.
    // @todo should use render arrays instead of actual output
    $item['slide'] = render($row);
    $item['caption'] = (isset($row['#caption']) && !empty($row['#caption'])) ? $row['#caption'] : NULL;
    $items[] = $item;
  }

  $content = array(
    '#theme' => 'flexslider',
    '#items' => $items,
    '#settings' => $settings,
    '#attributes' => array('id' => $variables['options']['id']),
  );

  // Add the slide items to the variables.
  $variables['content'] = $content;

}
