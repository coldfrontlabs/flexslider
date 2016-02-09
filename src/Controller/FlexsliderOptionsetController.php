<?php
/**
 * @file
 * Definition of Drupal\flexslider\Controller\FlexsliderOptionsetController.
 *
 * @author Agnes Chisholm <amaria@66428.no-reply.drupal.org>
 */

namespace Drupal\flexslider\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\flexslider\Entity\Flexslider;
use Symfony\Component\HttpFoundation\RedirectResponse;

/**
 * Route controller class for the flexslider module options configuration.
 */
class FlexsliderOptionsetController extends ControllerBase {

  /**
   * Enables a Flexslider object.
   *
   * @param \Drupal\flexslider\Entity\Flexslider $flexslider
   *   The Flexslider object to enable.
   *
   * @return \Symfony\Component\HttpFoundation\RedirectResponse
   *   A redirect response to the Flexslider optionset listing page.
   */
  public function enable(Flexslider $flexslider) {
    $flexslider->enable()->save();
    return new RedirectResponse($flexslider->url('collection', array('absolute' => TRUE)));
  }

  /**
   * Disables an Flexslider object.
   *
   * @param \Drupal\flexslider\Entity\Flexslider $flexslider
   *   The Flexslider object to disable.
   *
   * @return \Symfony\Component\HttpFoundation\RedirectResponse
   *   A redirect response to the Flexslider optionset listing page.
   */
  public function disable(Flexslider $flexslider) {
    $flexslider->disable()->save();
    return new RedirectResponse($flexslider->url('collection', array('absolute' => TRUE)));
  }

}
