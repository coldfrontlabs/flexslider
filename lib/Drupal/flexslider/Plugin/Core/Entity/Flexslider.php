<?php

/**
 * @file
 * Contains Drupal\flexslider\Plugin\Core\Entity\Flexslider
 *
 * @author Mathew Winstone <mwinstone@coldfrontlabs.ca>
 */

namespace Drupal\robot\Plugin\Core\Entity;

use Drupal\Core\Config\Entity\ConfigEntityBase;
use Drupal\Core\Annotation\Plugin;
use Drupal\Core\Annotation\Translation;
use Drupal\flexslider\FlexsliderInterface;

/**
 * Defines the robot entity.
 *
 * @Plugin(
 *   id = "flexslider",
 *   label = @Translation("Flexslider Optionset"),
 *   module = "robot",
 *   controller_class = "Drupal\flexslider\FlexsliderStorageController",
 *   list_controller_class = "Drupal\flexslider\FlexsliderListController",
 *   form_controller_class = {
 *     "default" = "Drupal\robot\FlexsliderFormController"
 *   },
 *   uri_callback = "flexslider_uri",
 *   config_prefix = "flexslider.optionset",
 *   entity_keys = {
 *     "id" = "name",
 *     "label" = "title",
 *     "uuid" = "uuid"
 *   }
 * )
 */
class Flexslider extends ConfigEntityBase implements FlexsliderInterface {
  /**
   * The machine name of this rule.
   *
   * @var string
   */
  public $name;

  /**
   * The UUID of this rule.
   *
   * @var string
   */
  public $uuid;

  /**
   * The human-readable label of this rule.
   *
   * @var string
   */
  public $title;

  /**
   * The human-readable description of this rule.
   *
   * @var string
   */
  public $description;

  /**
   * The stored optionset
   *
   * @var string
   */
  public $optionset;

  /**
   * Overrides \Drupal\Core\Entity\Entity::uri().
   */
  public function uri() {
    return array(
      'path' => 'admin/config/media/flexslider/manage/' . $this->id(),
      'options' => array(
        'entity_type' => $this->entityType,
        'entity' => $this,
      ),
    );
  }
}