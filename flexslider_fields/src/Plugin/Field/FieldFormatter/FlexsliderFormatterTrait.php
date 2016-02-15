<?php
/**
 * @file
 * Contains Drupal\flexslider_fields\FlexsliderFormatterTrait
 *
 * @author Agnes Chisholm <amaria@66428.no-reply.drupal.org>
 */

namespace Drupal\flexslider_fields\Plugin\Field\FieldFormatter;

use Drupal\Core\Field\FieldDefinitionInterface;
use Drupal\Core\Field\FormatterBase;
use Drupal\Core\Url;
use Drupal\Core\Cache\Cache;
use Drupal\Component\Utility\Xss;

/**
 * A common Trait for flexslider formatters.
 *
 * Currently, only image based formatters exist for flexslider but this trait
 * could apply to any type formatter.
 *
 * @see Drupal\Core\Field\FormatterBase
 */
trait FlexsliderFormatterTrait {

  /**
   * Returns the flexslider specific default settings.
   *
   * @return array
   */
  protected static function getDefaultSettings() {
    return array(
      'optionset' => 'default',
      'caption' => '',
    ) ;
  }

  /**
   * Builds the flexslider settings summary.
   *
   * @param \Drupal\Core\Field\FormatterBase $formatter
   *
   * @return array
   */
  protected function buildSettingsSummary(FormatterBase $formatter) {
    $summary = array();

    // Load the selected optionset.
    $optionset = $this->loadOptionset($formatter->getSetting('optionset'));

    // Build the optionset summary.
    $os_summary = $optionset ? $optionset->label() : $formatter->t('Default settings');
    $summary[] = $formatter->t('Option set: %os_summary', array('%os_summary' => $os_summary));

    return $summary;
  }

  /**
   * Builds the flexslider settings form.
   *
   * @param \Drupal\Core\Field\FormatterBase $formatter
   * @param array $formatter_settings
   *
   * @return array
   */
  protected function buildSettingsForm(FormatterBase $formatter) {

    // Get list of option sets as an associative array.
    $optionsets = flexslider_optionset_list();

    $element['optionset'] = array(
      '#title' => $formatter->t('Option Set'),
      '#type' => 'select',
      '#default_value' => $formatter->getSetting('optionset'),
      '#options' => $optionsets,
    );

    $element['links'] = array(
      '#theme' => 'links',
      '#links' => array(
        array(
          'title' => $formatter->t('Create new option set'),
          'url' => Url::fromRoute('entity.flexslider.add_form', array(), array('query' => \Drupal::destination()->getAsArray())),
        ),
        array(
          'title' => $formatter->t('Manage option sets'),
          'url' => Url::fromRoute('entity.flexslider.collection', array(), array('query' => \Drupal::destination()->getAsArray())),
        ),
      ),
      '#access' => \Drupal::currentUser()->hasPermission('administer flexslider'),
    );

    return $element;
  }

  /**
   * The flexslider formatted view for images.
   *
   * @param array $images
   * @param array $formatter_settings
   *
   * @return array
   */
  protected function viewImages(array $images, array $formatter_settings) {

    // Bail out if no images to render.
    if (empty($images)) {
      return array();
    }

    // Get cache tags for the option set.
    if ($optionset = $this->loadOptionset($formatter_settings['optionset'])) {
      $cache_tags = $optionset->getCacheTags();
    }
    else {
      $cache_tags = array();
    }

    $items = [];

    foreach ($images as $delta => &$image) {

      // Merge in the cache tags.
      if ($cache_tags) {
        $image['#cache']['tags'] = Cache::mergeTags($image['#cache']['tags'], $cache_tags);
      }

      // Prepare the slide item render array.
      $item = array();
      // @todo Should find a way of dealing with render arrays instead of the actual output
      $item['slide'] = render($image);

      // Check caption settings.
      if ($formatter_settings['caption'] == 1) {
        $item['caption'] = ['#markup' => Xss::filterAdmin($image['#item']->title)];
      }
      elseif ($formatter_settings['caption'] == 'alt') {
        $item['caption'] = ['#markup' => Xss::filterAdmin($image['#item']->alt)];
      }

      $items[$delta] = $item;
    }

    // We have to pass an array of elements for Views compatibility.
    $elements[] = array(
      '#theme' => 'flexslider',
      '#items' => $items,
      '#settings' => $formatter_settings,
    );

    return $elements;
  }

  /**
   * Loads the selected option set.
   *
   * @param string $id
   *
   * @returns \Drupal\flexslider\Entity\Flexslider
   *   The option set selected in the formatter settings.
   */
  protected function loadOptionset($id) {
      return \Drupal\flexslider\Entity\Flexslider::load($id);
  }

  /**
   * Returns the form element for caption settings.
   *
   * @param \Drupal\Core\Field\FormatterBase $formatter
   * @param \Drupal\Core\Field\FieldDefinitionInterface $field_definition
   *
   * @return array
   */
  protected function captionSettings(FormatterBase $formatter, FieldDefinitionInterface $field_definition) {
    $field_settings = $field_definition->getSettings();

    // Set the caption options.
    $caption_options = array(
      0 => $formatter->t('None'),
      1 => $formatter->t('Image title'),
      'alt' => $formatter->t('Image ALT attribute'),
    );

    // Remove the options that are not available.
    $action_fields = array();
    if ($field_settings['title_field'] == FALSE) {
      unset( $caption_options[1]);
      // User action required on the image title.
      $action_fields[] = 'title';
    }
    if ($field_settings['alt_field'] == FALSE) {
      unset( $caption_options['alt']);
      // User action required on the image alt.
      $action_fields[] = 'alt';
    }

    // Create the caption element.
    $element['caption'] = array(
      '#title' => $formatter->t('Choose a caption source'),
      '#type' => 'select',
      '#options' => $caption_options,
    );

    // If the image field doesn't have all of the suitable caption sources, tell the user.
    if ($action_fields) {
      $action_text = $formatter->t('enable the @action_field field', array('@action_field' => join(' and/or ', $action_fields)));
      /* This may be a base field definition (e.g. in Views UI) which means it
       * is not associated with a bundle and will not have the toUrl() method.
       * So we need to check for the existence of the method before we can
       * build a link to the image field edit form.
       */
      if (method_exists($field_definition, 'toUrl')) {
        // Build the link to the image field edit form for this bundle.
        $rel = "{$field_definition->getTargetEntityTypeId()}-field-edit-form";
        $action = $field_definition->toLink($action_text, $rel,
          array(
            'fragment' => 'edit-settings-alt-field',
            'query' => \Drupal::destination()->getAsArray(),
          )
        )->toRenderable();
      }
      else {
        // Just use plain text if we can't build the field edit link.
        $action = ['#markup' => $action_text];
      }
      $element['caption']['#description']
        = $formatter->t('You need to @action for this image field to be able to use it as a caption.',
        array('@action' => render($action)));

      // If there are no suitable caption sources, disable the caption element.
      if (count($action_fields) >= 2) {
        $element['caption']['#disabled'] = TRUE;
      }
    }
    else {
      $element['caption']['#default_value'] = $formatter->getSetting('caption');
    }

    return $element;
  }
}