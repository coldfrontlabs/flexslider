<?php

namespace Drupal\flexslider\Form;

use Drupal\Core\Asset\LibraryDiscovery;
use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Session\AccountProxyInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Class FlexsliderFormSettings.
 *
 * @package Drupal\flexslider\Form
 */
class FlexsliderFormSettings extends ConfigFormBase {

  /**
   * The library discovery service.
   *
   * @var LibraryDiscovery
   */
  private $libraryDiscovery;

  /**
   * The current user account service.
   *
   * @var AccountProxyInterface
   */
  private $currentUser;

  /**
   * Constructs a new FlexSliderFormSettings.
   *
   * @param \Drupal\Core\Config\ConfigFactoryInterface $configFactory
   *   The config factory service.
   * @param \Drupal\Core\Asset\LibraryDiscovery $libraryDiscovery
   *   The library discovery service.
   * @param \Drupal\Core\Session\AccountProxyInterface $currentUser
   *   The user account service.
   */
  public function __construct(ConfigFactoryInterface $configFactory, LibraryDiscovery $libraryDiscovery, AccountProxyInterface $currentUser) {
    parent::__construct($configFactory);
    $this->libraryDiscovery = $libraryDiscovery;
    $this->currentUser = $currentUser;
  }

  /**
   * Use Symfony's ContainerInterface to declare dependency for constructor.
   *
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('config.factory'),
      $container->get('library.discovery'),
      $container->get('current_user')
    );
  }

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
    $this->libraryDiscovery->clearCachedDefinitions();

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
  public function buildForm(array $form, FormStateInterface $form_state) {
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
      '#default_value' => $this->config('flexslider.settings')->get('flexslider_debug'),
      '#access' => $this->currentUser->hasPermission('administer flexslider'),
    ];

    return parent::buildForm($form, $form_state);
  }

}
