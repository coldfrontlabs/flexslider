<?php

/**
 * @file
 * Template for the FlexSlider row container
 *
 * @author Mathew Winstone (minorOffense) <mwinstone@coldfrontlabs.ca>
 */
?>
<div id="flexslider-<?php print $variables['vss_id']; ?>" class="flexslider">
  <ul class="<?php print $classes; ?>">
    <?php print $rendered_rows; ?>
  </ul>
</div>
