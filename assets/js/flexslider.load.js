(function ($) {

// Behavior to load FlexSlider
Drupal.behaviors.flexslider = {
  attach: function(context, settings) {
    for (id in settings.flexslider.instances) {
      $('#' + id, context).once('flexslider', function() {
        if (settings.flexslider.optionsets[settings.flexslider.instances[id]] !== undefined) {
          // Remove width/height attributes
          // @todo load the css path from the settings
          $(this).find('ul.slides > li > img').removeAttr('height');
          $(this).find('ul.slides > li > img').removeAttr('width');
          
          var optionset = settings.flexslider.optionsets[settings.flexslider.instances[id]];
          if (optionset) {
            $(this).flexslider(optionset);
          }
          else {
            $(this).flexslider();
          }
        }
      });
    }
  }
};

}(jQuery));
