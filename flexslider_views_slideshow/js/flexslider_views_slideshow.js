(function ($) {
  // @todo convert to use Drupal.behaviors
  // @todo add configuration options

  // Register callback to save references to flexslider instances. Allows
  // Views Slideshow controls to affect the slider
  function flexslider_views_slideshow_register(fullId, slider) {
    Drupal.flexsliderViewsSlideshow.active = Drupal.flexsliderViewsSlideshow.active || {};
    Drupal.flexsliderViewsSlideshow.active[fullId] = slider;
  }

  Drupal.behaviors.flexsliderViewsSlideshow = {
    attach: function (context) {
      $('.flexslider_views_slideshow_main:not(.flexslider_views_slideshow-processed)', context).addClass('flexslider_views_slideshow-processed').each(function() {
        // Get the ID of the slideshow
        var fullId = '#' + $(this).attr('id');

        // Create settings container
        var settings = Drupal.settings.flexslider_views_slideshow[fullId];

        //console.log(settings);

        // @todo map the settings from the form to their javascript equivalents
        settings.targetId = fullId;
        
        settings.loaded = false;

        // Assign default settings
        settings.opts = {
          animation:settings.animation,
          slideDirection:settings.slidedirection,
          slideshow:settings.slideshow,
          slideshowSpeed:settings.slideshowspeed,
          animationDuration:settings.animationduration,
          directionNav:settings.directionnav,
          controlNav:settings.controlnav,
          keyboardNav:settings.keyboardnav,
          mousewheel:settings.mousewheel,
          prevText:settings.prevtext,
          nextText:settings.nexttext,
          pausePlay:settings.pauseplay,
          pauseText:settings.pausetext,
          playText:settings.playtext,
          randomize:settings.randomize,
          slideToStart:settings.slidetostart,
          animationLoop:settings.animationloop,
          pauseOnAction:settings.pauseonaction,
          pauseOnHover:settings.pauseonhover,
          controlsContainer:settings.controlscontainer,
          manualControls:settings.manualcontrols,
          start: function(slider) {
            flexslider_views_slideshow_register(fullId, slider);
          }
        };

        Drupal.flexsliderViewsSlideshow.load(fullId);
      });
    }
  };


  // Initialize the flexslider object
  Drupal.flexsliderViewsSlideshow = Drupal.flexsliderViewsSlideshow || {};

  // Load mapping from Views Slideshow to FlexSlider
  Drupal.flexsliderViewsSlideshow.load = function(fullId) {
    var settings = Drupal.settings.flexslider_views_slideshow[fullId];

    // Ensure the slider isn't already loaded
    if (!settings.loaded) {
      $(settings.targetId + " .flexslider").flexslider(settings.opts);
      settings.loaded = true;
    }
  }

  // Pause mapping from Views Slideshow to FlexSlider
  Drupal.flexsliderViewsSlideshow.pause = function (options) {
    Drupal.flexsliderViewsSlideshow.active['#flexslider_views_slideshow_main_' + options.slideshowID].pause();
    Drupal.flexsliderViewsSlideshow.active['#flexslider_views_slideshow_main_' + options.slideshowID].manualPause = true;
  }

  // Play mapping from Views Slideshow to FlexSlider
  Drupal.flexsliderViewsSlideshow.play = function (options) {
    Drupal.flexsliderViewsSlideshow.active['#flexslider_views_slideshow_main_' + options.slideshowID].resume();
    Drupal.flexsliderViewsSlideshow.active['#flexslider_views_slideshow_main_' + options.slideshowID].manualPause = false;
  }
  
  Drupal.flexsliderViewsSlideshow.nextSlide = function (options) {
    //Drupal.flexsliderViewsSlideshow.active['#flexslider_views_slideshow_main_' + options.slideshowID].resume();
  }
  Drupal.flexsliderViewsSlideshow.previousSlide = function (options) {
    //Drupal.flexsliderViewsSlideshow.active['#flexslider_views_slideshow_main_' + options.slideshowID].resume();
  }
  // @todo add support for jquery mobile page init
})(jQuery);