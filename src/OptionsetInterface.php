<?php

/**
 * @file
 * Contains \Drupal\flexslider\OptionsetInterface.
 */

namespace Drupal\flexslider;

use Drupal\Core\Config\Entity\ConfigEntityInterface;

/**
 * Provides an interface for defining Flexslider optionset entities.
 */
interface OptionsetInterface extends ConfigEntityInterface {
  public function getOptions();
  public function setOptions($options);
}
