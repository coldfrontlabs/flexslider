<?php
/**
 * @file
 * Contains \Drupal\flexslider_fields\Plugin\Field\FieldFormatter\FlexsliderFormatter.
 *
 * @author Agnes Chisholm <amaria@66428.no-reply.drupal.org>
 */

namespace Drupal\flexslider_fields\Plugin\Field\FieldFormatter;

use Drupal\image\Plugin\Field\FieldFormatter\ImageFormatter;

/**
 * Plugin implementation of the '<flexslider>' formatter.
 *
 * @FieldFormatter(
 *   id = "flexslider",
 *   label = @Translation("FlexSlider"),
 *   field_types = {
 *     "image",
 *     "media"
 *   }
 * )
 */
class FlexsliderFormatter extends ImageFormatter {
  use FlexsliderFormatterTrait;

}
