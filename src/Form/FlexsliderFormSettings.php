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

/**
 * Class FlexsliderFormSettings.
 *
 * @package Drupal\flexslider\Form
 */
class FlexsliderFormSettings extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'flexslider_advanced_settings_form';
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('flexslider.settings');
    $config->set('flexslider_debug', $form_state->getValue('flexslider_debug'))
              ->save();

    // Invalidate the library discovery cache to update new assets.
    \Drupal::service('library.discovery')->clearCachedDefinitions();

    parent::submitForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return ['flexslider.settings'];
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, \Drupal\Core\Form\FormStateInterface $form_state) {
    $form = [];

    $form['library'] = [
      '#type' => 'fieldset',
      '#title' => 'Library',
      '#tree' => FALSE,
    ];

    // Debug mode toggle.
    $form['library']['flexslider_debug'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Enable debug mode'),
      '#description' => $this->t('Load the human-readable version of the FlexSlider library.'),
      '#default_value' => \Drupal::config('flexslider.settings')->get('flexslider_debug'),
      '#access' => \Drupal::currentUser()->hasPermission('administer flexslider'),
    ];

    return parent::buildForm($form, $form_state);
  }

}
