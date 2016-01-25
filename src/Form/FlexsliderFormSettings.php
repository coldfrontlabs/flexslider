<?php

/**
 * @file
 * Contains \Drupal\flexslider\Form\FlexsliderFormSettings.
 *
 * @author Agnes Chisholm <amaria@66428.no-reply.drupal.org>
 */

namespace Drupal\flexslider\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Render\Element;

class FlexsliderFormSettings extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'flexslider_form_settings';
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('flexslider.settings');

    foreach (Element::children($form) as $variable) {
      $config->set($variable, $form_state->getValue($form[$variable]['#parents']));
    }
    $config->save();

    if (method_exists($this, '_submitForm')) {
      $this->_submitForm($form, $form_state);
    }

    parent::submitForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return ['flexslider.settings'];
  }

  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $form = [];

    $form['library'] = [
      '#type' => 'fieldset',
      '#title' => 'Library',
    ];

    // Debug mode toggle
    $form['library']['flexslider_debug'] = [
      '#type' => 'checkbox',
      '#title' => t('Enable debug mode'),
      '#description' => t('Load the human-readable version of the FlexSlider library.'),
      '#default_value' => \Drupal::config('flexslider.settings')->get('flexslider_debug'),
      '#access' => \Drupal::currentUser()->hasPermission('administer flexslider'),
    ];

    return parent::buildForm($form, $form_state);
  }

  public function _submitForm(array &$form, \Drupal\Core\Form\FormStateInterface $form_state) {
    // Do nothing for now
  }

}
