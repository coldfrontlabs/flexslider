<?php
/**
 * @file
 * Default output for a galleria node.

*/
?>
<div class="flexslider-content flexslider clearfix" id="flexslider-<?php print $id; ?>">
  <ul class="slides">
  
  <?php /*dpm($items);*/?>
  
  <?php foreach($items as $item) { ?>
    <li><?php print render($item); ?>
       


  <?php if($item['#item']['title'] != '' || $item['#item']['alt'] != '') { ?>
       <div class="flex-caption"><strong><?php print $item['#item']['title']; ?></strong>&nbsp;<?php print $item['#item']['alt'];?></div>
  <?php } ?>     
 
  </li>
 
  <?php } ?>   
  </ul>

</div>




<?php /* foreach($items as $item) if(!empty($item['#item']['alt'])) { 
    <li>print $item['#item']['alt'];</li>
   }
 ?>
   <?php if(!empty($item['#item']['alt'])) {
   
   <?php if(strnlen($item['#item']['alt'])) > 0)
   
   <div class="flex-caption"><?php print $item['#item']['title'];?></div>

<?php /*
<div class="flex-caption"><li><?php print $item['#item']['alt']; ?></li></div>
*/     
?>
