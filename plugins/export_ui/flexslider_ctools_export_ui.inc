<?php

/**
 * @file
 * Export interface plugin
 *
 * @author Mathew Winstone <mwinstone@coldfrontlabs.ca>
 */


/**
* Define this Export UI plugin.
*/
$plugin = array(
  'schema' => 'flexslider_optionset',  // As defined in hook_schema().
  'access' => 'administer flexslider',  // Define a permission users must have to access these pages.

  // Define the menu item.
  'menu' => array(
    'menu prefix' => 'admin/config/media',
    'menu item' => 'flexslider',
    'menu title' => 'FlexSlider',
    'menu description' => 'Administer FlexSlider presets.',
  ),

  // Define user interface texts.
  'title singular' => t('optionset'),
  'title plural' => t('optionsets'),
  'title singular proper' => t('FlexSlider optionset'),
  'title plural proper' => t('FlexSlider optionsets'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'flexslider_ctools_export_ui_form',
    'validate' => 'flexslider_ctools_export_ui_form_validate',
    'submit' => 'flexslider_ctools_export_ui_form_submit',
  ),
);

/**
 * Export UI form
 */
function flexslider_ctools_export_ui_form(&$form, &$form_state) {
  // Load the admin form include
  module_load_include('inc', 'flexslider', 'flexslider.admin');

  // Make optionset reference in form_state
  $form_state['optionset'] = &$form_state['item'];

  // Load the configuration form
  $form = drupal_retrieve_form('flexslider_form_optionset_edit', $form_state);
}

/**
 * Validation handler
 */
function flexslider_ctools_export_ui_form_validate(&$form, &$form_state) {
  // @todo
}

/**
 * Submit handler
 */
function flexslider_ctools_export_ui_form_submit(&$form, &$form_state) {
  // Put the optionset into the 'item' form_state key so CTools can find it
}