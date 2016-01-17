<?php
/**
 * @file
 * Contains \Drupal\flexslider_fields\Plugin\Field\FieldFormatter\FlexsliderFormatter.
 */
namespace Drupal\flexslider_fields\Plugin\Field\FieldFormatter;

use Drupal\image\Plugin\Field\FieldFormatter\ImageFormatter;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Field\FieldItemListInterface;

/**
 * Plugin implementation of the '<field_formatter_id>' formatter.
 *
 * @FieldFormatter(
 *   id = "flexslider",
 *   label = @Translation("Flexslider"),
 *   field_types = {
 *     "image",
 *     "media"
 *   }
 * )
 */
class FlexsliderFormatter extends ImageFormatter {

  /**
   * {@inheritdoc}
   */
  public function settingsSummary() {
    $summary = array();
    $settings = $this->getSettings();

    // Load the selected optionset
    $optionset_id = $this->getSetting('optionset');
    /** @var   $optionset \Drupal\flexslider\OptionsetInterface */
    $optionset = NULL;
    if ($optionset_id) {
      $o = \Drupal\flexslider\Entity\Optionset::load($optionset_id);
      if ($o !== NULL) {
        $optionset = $o;
      }
    }

    // Build the settings summary
    $os_summary = $optionset ? $optionset->label() : t('Default settings');
    $summary[] = t('Option set: %os_summary', array('%os_summary' => $os_summary));

    // Add the image settings summary and return
    return array_merge($summary, parent::settingsSummary());
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm(array $form, FormStateInterface $form_state) {

    $optionsets = flexslider_optionset_load_all();
    $element['optionset'] = array(
      '#title' => t('Option Set'),
      '#type' => 'select',
      '#default_value' => $this->getSetting('optionset'),
      '#options' => $optionsets,
    );
/*
    $image_styles = image_style_options(FALSE);
    $element['image_style'] = array(
      '#title' => t('Image style'),
      '#type' => 'select',
      '#default_value' => $this->getSetting('image_style'),
      '#empty_option' => t('None (original image)'),
      '#options' => $image_styles,
    );
*/
    $element = array_merge($element, parent::settingsForm($form, $form_state));

    $field_settings = $this->getFieldSettings();
    if (!empty($field_settings)) {
      $element['caption'] = array(
        '#title' => t('Use image title as the caption'),
        '#type' => 'checkbox',
      );

      // If the image field doesn't have the Title field enabled, tell the user.
      if ($field_settings['title_field'] == FALSE) {
        // Build the field config url
        $rel = "{$this->fieldDefinition->getTargetEntityTypeId()}-field-edit-form";
        $action = \Drupal\Core\Link::fromTextAndUrl(
          $this->t('enable the Title field'),
          $this->fieldDefinition->toUrl($rel,
            array(
              'fragment' => 'edit-settings-alt-field',
              'query' => \Drupal::destination()->getAsArray()
            )
          )
        );

        $element['caption']['#disabled'] = TRUE;
        $element['caption']['#description'] =
            t('You need to @action for this image field to be able to use it as a caption.',
              array('@action' => render($action->toRenderable())));
      }
      else {
        $element['caption']['#default_value'] = $this->getSetting('caption');
      }
    }

    return $element;
  }

  /**
   * {@inheritdoc}
   */
  public function viewElements(FieldItemListInterface $items, $langcode) {
    // TODO: Implement viewElements() method.
  }
}