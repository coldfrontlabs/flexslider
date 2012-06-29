<?php
/**
 * @file
 * Default output for a Flex Slider node.
*/
?>
<div class="flex-nav-container">
  <div class="flexslider-content flexslider clearfix" id="flexslider-<?php print $id; ?>">
    <ul class="slides">
    <?php foreach($items as $item) : ?>
      <li>
        <?php
          if (isset($item['#item']['image'])) {
            print $item['#item']['image'];
          }
          else {
            print render($item);
          }
        ?>
        <?php if(isset($item['#item']['title']) || isset($item['#item']['alt'])) : ?>
          <div class="flex-caption">
            <?php if(!empty($item['#item']['title'])) : ?>
              <strong><?php print $item['#item']['title']; ?></strong>
            <?php endif; ?>
            <?php if(!empty($item['#item']['alt'])) : ?>
              <?php print $item['#item']['alt']; ?>
            <?php endif; ?>
          </div>
        <?php endif; ?>
      </li>
    <?php endforeach; ?>
    </ul>
  </div>
</div>
