<?php
/**
 * @file
 * Theming functions for the flexslider module.
 *
 * Preprocessor functions fill variables for templates and helper
 * functions to make theming easier.
 */
use Drupal\Component\Utility\NestedArray;
use Drupal\Core\Template\Attribute;
use Drupal\flexslider\Entity\Flexslider;


/**
 * Template preprocess handler for 'flexslider' theme.
 */
function template_preprocess_flexslider(&$vars) {

  // Reference configuration variables
  $optionset = &$vars['settings']['optionset'];
  $settings = &$vars['settings'];
  $items = &$vars['items'];

  // Set the default container type
  if (empty($settings['type'])) {
    $settings['type'] = 'ul';
  }

  // Load the selected optionset
  if (!empty($optionset)) {
    $optionset = Flexslider::load($optionset);
  }

  // Check if an optionset was loaded
  if (is_null($optionset)) {
    // Fall back to 'default' options
    $options = \Drupal::config('flexslider.optionset.default')->get('options');
    \Drupal::logger('flexslider')->warning('Fallback to default optionset.', array());
  }
  else {
    $options = $optionset->getOptions();
  }

  // Configure attributes for containing elements
  $attributes = $vars['attributes'];
  // Merge with defined attributes
  if (isset($settings['attributes']) && is_array($settings['attributes'])) {
    $attributes = NestedArray::mergeDeep($attributes, $settings['attributes']);
  }

  // Set the ID for each flexslider instance if none is provided
  if (empty($attributes['id'])) {
    $flexslider_id = &drupal_static('flexslider_id', 0);
    $attributes['id'] = 'flexslider-' . ++$flexslider_id;
  }

  // Add the namespace to any classes
  // @todo figure out what this is supposed to do
  if (!empty($attributes['class']) && !empty($options['namespace'])) {
    foreach ($attributes['class'] as $key => $value) {
      $attributes['class'][$key] = $options['namespace'] . $value;
    }
  }

  // Add the flexslider class to be namespaced
  $attributes['class'][] = 'flexslider';

  // Pass attributes to twig.
  $vars['attributes'] = $attributes;

  // Add the list render array
  $vars['content']['list'] = array(
    '#theme' => 'flexslider_list',
    '#items' => $items,
    '#settings' => $settings,
  );

  // Finally, add the configuration to the page
  $attached = flexslider_add($vars['attributes']['id'], $vars['settings']['optionset']);
  $vars['#attached'] = $attached;

}

/**
 * Preprocess function for flexslider_list
 */
function template_preprocess_flexslider_list(&$vars) {

  // Reference configuration variables
  $optionset = &$vars['settings']['optionset'];
  $items = &$vars['items'];
  $attributes = &$vars['attributes'];

  // @todo find a way to detect the outer container class if possible
  $attributes['class'][] =  'slides';

  // Build the list
  $vars['content']['list'] = array();
  if (!empty($items)) {
    foreach ($items as $i => $item) {

      $caption = '';
      if (!empty($item['caption'])) {
        $caption = $item['caption'];
      }

      // Add the list item render array
      $vars['content']['list'][$i] = array(
        '#theme' => 'flexslider_list_item',
        '#item' => $item['slide'],
        '#settings' => array(
           'optionset' => $optionset,
         ),
         '#caption' => $caption,
      );
    }
  }

}

/**
 * Preprocess function for flexslider_list_item
 */
function template_preprocess_flexslider_list_item(&$vars) {

  // Reference configuration variables
  $item = &$vars['item'];
  $settings = &$vars['settings'];
  $caption = &$vars['caption'];
  $attributes = &$vars['attributes'];
  /** @var \Drupal\flexslider\Entity\Flexslider $optionset */
  $optionset = $settings['optionset'];

  // Generated thumbnail support
  if ($optionset->getOption('controlNav') === "thumbnails") {
    // If the thumbnails are enabled in the option set,
    // extract the url to set as the thumbnail data
    // @todo deal with caching when optionset changes
    // @fixme make sure we use the style url instead of the original if style name set
    $attributes['data-thumb'] = $item['#item']->entity->url();
  }

  if ($optionset->getOption('thumbCaptions') and !empty($caption)) {
    $attributes['data-thumbcaption'] = $caption;
    // Prevent captions from appearing in the slider as well
    if (FALSE === $optionset->getOption('thumbCaptionsBoth')) {
      $caption = '';      
    }
  }


}
