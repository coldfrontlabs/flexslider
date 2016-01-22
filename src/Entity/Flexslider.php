<?php

/**
 * @file
 * Contains \Drupal\flexslider\Entity\Flexslider.
 */

namespace Drupal\flexslider\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBase;
use Drupal\flexslider\FlexsliderInterface;

/**
 * Defines the Flexslider entity.
 *
 * @ConfigEntityType(
 *   id = "flexslider",
 *   label = @Translation("Flexslider optionset"),
 *   handlers = {
 *     "list_builder" = "Drupal\flexslider\Controller\FlexsliderListBuilder",
 *     "form" = {
 *       "add" = "Drupal\flexslider\Form\FlexsliderForm",
 *       "edit" = "Drupal\flexslider\Form\FlexsliderForm",
 *       "delete" = "Drupal\flexslider\Form\FlexsliderDeleteForm"
 *     }
 *   },
 *   config_prefix = "optionset",
 *   admin_permission = "administer flexslider",
 *   entity_keys = {
 *     "id" = "id",
 *     "label" = "label",
 *     "status" = "status"
 *   },
 *   links = {
 *     "canonical" = "/admin/config/media/flexslider/{flexslider}",
 *     "edit-form" = "/admin/config/media/flexslider/{flexslider}/edit",
 *     "enable" = "/admin/config/media/flexslider/{flexslider}/enable",
 *     "disable" = "/admin/config/media/flexslider/{flexslider}/disable",
 *     "delete-form" = "/admin/config/media/flexslider/{flexslider}/delete",
 *     "collection" = "/admin/config/media/flexslider"
 *   },
 *   config_export = {
 *     "id",
 *     "label",
 *     "options",
 *   }
 * )
 */
class Flexslider extends ConfigEntityBase implements FlexsliderInterface {
  /**
   * The Flexslider optionset ID.
   *
   * @var string
   */
  protected $id;

  /**
   * The Flexslider optionset label.
   *
   * @var string
   */
  protected $label;

  /**
   * The Flexslider optionset options.
   *
   * @var array
   */
  protected $options = array();

  /**
   * {@inheritdoc}
   */
  public function getOptions() {
    return $this->options;
  }

  /**
   * {@inheritdoc}
   */
  public function setOptions($options) {
    $this->options = $options;
  }

  /**
   * {@inheritdoc}
   */
  public function getOption($name) {
    return isset($this->options[$name]) ? $this->options[$name] : NULL;
  }

  /**
   * @param mixed $id
   * @return \Drupal\flexslider\Entity\Flexslider
   */
  public static function load($id) {
    return parent::load($id);
  }

}