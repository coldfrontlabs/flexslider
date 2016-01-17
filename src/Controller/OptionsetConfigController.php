<?php

namespace Drupal\flexslider\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\flexslider\Entity\Optionset;
use Symfony\Component\HttpFoundation\RedirectResponse;

/**
 * Route controller class for the flexslider module options configuration.
 */
class OptionsetConfigController extends ControllerBase {

  /**
   * Enables an Optionset object.
   *
   * @param \Drupal\flexslider\Entity\Optionset $flexslider_optionset
   *   The Optionset object to enable.
   *
   * @return \Symfony\Component\HttpFoundation\RedirectResponse
   *   A redirect response to the Flexslider optionset listing page.
   */
  function enable(Optionset $flexslider_optionset) {
    $flexslider_optionset->enable()->save();
    return new RedirectResponse($flexslider_optionset->url('collection', array('absolute' => TRUE)));
  }

  /**
   * Disables an Optionset object.
   *
   * @param \Drupal\flexslider\Entity\Optionset $flexslider_optionset
   *   The Optionset object to disable.
   *
   * @return \Symfony\Component\HttpFoundation\RedirectResponse
   *   A redirect response to the Flexslider optionset listing page.
   */
  function disable(Optionset $flexslider_optionset) {
    $flexslider_optionset->disable()->save();
    return new RedirectResponse($flexslider_optionset->url('collection', array('absolute' => TRUE)));
  }

}