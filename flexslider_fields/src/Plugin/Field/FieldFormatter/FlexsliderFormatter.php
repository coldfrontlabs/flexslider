<?php
/**
 * @file
 * Contains \Drupal\flexslider_fields\Plugin\Field\FieldFormatter\FlexsliderFormatter.
 */
namespace Drupal\flexslider_fields\Plugin\Field\FieldFormatter;

use Drupal\Core\Url;
use Drupal\flexslider\Entity\Flexslider;
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

    // Load the selected optionset
    $optionset = $this->loadOptionset();

    // Build the optionset summary
    $os_summary = $optionset ? $optionset->label() : t('Default settings');
    $summary[] = t('Option set: %os_summary', array('%os_summary' => $os_summary));

    // Add the image settings summary and return
    return array_merge($summary, parent::settingsSummary());
  }

  /**
   * {@inheritdoc}
   */
  public function settingsForm(array $form, FormStateInterface $form_state) {

    // Get list of option sets as an associative array
    $optionsets = flexslider_optionset_load_all();

    $element['optionset'] = array(
      '#title' => t('Option Set'),
      '#type' => 'select',
      '#default_value' => $this->getSetting('optionset'),
      '#options' => $optionsets,
    );

    $element['links'] = array(
      '#theme' => 'links',
      '#links' => array(
        array(
          'title' => t('Create new option set'),
          'url' => Url::fromRoute('entity.flexslider.add_form', array(), array('query' => \Drupal::destination()->getAsArray())),
        ),
        array(
          'title' => t('Manage option sets'),
          'url' => Url::fromRoute('entity.flexslider.collection', array(), array('query' => \Drupal::destination()->getAsArray())),
        ),
      ),
      '#access' => $this->currentUser->hasPermission('administer flexslider'),
    );

    // Add the image settings
    $element = array_merge($element, parent::settingsForm($form, $form_state));
    // We don't need the link setting
    $element['image_link']['#access'] = FALSE;

    $field_settings = $this->getFieldSettings();
    if (!empty($field_settings)) {
      $element['caption'] = array(
        '#title' => t('Use image title as the caption'),
        '#type' => 'checkbox',
      );

      // If the image field doesn't have the Title field enabled, tell the user.
      if ($field_settings['title_field'] == FALSE) {
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
    $items = parent::viewElements($items, $langcode);

    // Bail out if no images to render
    if (empty($items)) {
      return array();
    }

    // Get cache tags for the option set
    if ($optionset = $this->loadOptionset()) {
      $tags = $optionset->getCacheTags();
    }
    else {
      $tags = array();
    }

    $element = array(
      '#theme' => 'flexslider',
      '#items' => $items,
      '#settings' => $this->getSettings(),
      '#cache' => $tags,
    );

    return $element;
  }

  /**
   * Loads the selected option set.
   *
   * @returns \Drupal\flexslider\Entity\Flexslider
   *   The option set selected in the formatter settings.
   */
  protected function loadOptionset() {
    if ($id = $this->getSetting('optionset')) {
      return Flexslider::load($id);
    }
    return NULL;
  }

}

