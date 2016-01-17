<?php

/**
 * @file
 * Contains \Drupal\flexslider\Entity\Optionset.
 */

namespace Drupal\flexslider\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBase;
use Drupal\flexslider\OptionsetInterface;

/**
 * Defines the Flexslider entity.
 *
 * @ConfigEntityType(
 *   id = "flexslider_optionset",
 *   label = @Translation("Flexslider"),
 *   handlers = {
 *     "list_builder" = "Drupal\flexslider\Controller\OptionSetListBuilder",
 *     "form" = {
 *       "add" = "Drupal\flexslider\Form\OptionSetForm",
 *       "edit" = "Drupal\flexslider\Form\OptionSetForm",
 *       "delete" = "Drupal\flexslider\Form\OptionSetDeleteForm"
 *     }
 *   },
 *   config_prefix = "flexslider_optionset",
 *   admin_permission = "administer flexslider",
 *   entity_keys = {
 *     "id" = "id",
 *     "label" = "label",
 *     "status" = "status"
 *   },
 *   links = {
 *     "canonical" = "/admin/config/media/flexslider/{flexslider_optionset}",
 *     "edit-form" = "/admin/config/media/flexslider/{flexslider_optionset}/edit",
 *     "enable" = "/admin/config/media/flexslider/{flexslider_optionset}/enable",
 *     "disable" = "/admin/config/media/flexslider/{flexslider_optionset}/disable",
 *     "delete-form" = "/admin/config/media/flexslider/{flexslider_optionset}/delete",
 *     "collection" = "/admin/config/media/flexslider"
 *   },
 *   config_export = {
 *     "id",
 *     "label",
 *     "options",
 *   }
 * )
 */
class Optionset extends ConfigEntityBase implements OptionsetInterface {
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
   * @return array
   */
  public function getOptions() {
    return $this->options;
  }

  /**
   * @param array $options
   */
  public function setOptions($options) {
    $this->options = $options;
  }

}
