<?php
/**
 * @file
 * Contains \Drupal\flexslider_fields\Plugin\Field\FieldFormatter\FlexsliderResponsiveFormatter.
 *
 * @author Agnes Chisholm <amaria@66428.no-reply.drupal.org>
 */

namespace Drupal\flexslider_fields\Plugin\Field\FieldFormatter;

use Drupal\Core\Field\FieldDefinitionInterface;
use Drupal\responsive_image\Plugin\Field\FieldFormatter\ResponsiveImageFormatter;

/**
 * Plugin implementation of the '<flexslider_responsive>' formatter.
 *
 * @FieldFormatter(
 *   id = "flexslider_responsive",
 *   label = @Translation("FlexSlider Responsive"),
 *   field_types = {
 *     "image",
 *     "media"
 *   }
 * )
 */
class FlexsliderResponsiveFormatter extends ResponsiveImageFormatter {
  use FlexsliderFormatterTrait;

}
