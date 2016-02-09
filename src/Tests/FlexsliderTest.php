<?php
/**
 * @file
 * Test cases for FlexSlider.
 *
 * @author Mathew Winstone <mwinstone@coldfrontlabs.ca>
 * @author Agnes Chisholm <amaria@chisholmtech.com>
 */

namespace Drupal\flexslider\Tests;

use Drupal\flexslider\Entity\Flexslider;
use Drupal\flexslider\FlexsliderDefaults;
use Drupal\simpletest\WebTestBase;

/**
 * Test the FlexSlider presets, configuration options and permission controls.
 *
 * @group flexslider
 */
class FlexsliderTest extends WebTestBase {

  /**
   * Our module dependencies.
   *
   * In Drupal 8's SimpleTest, we declare module dependencies in a public
   * static property called $modules. WebTestBase automatically enables these
   * modules for us.
   *
   * @var array
   */
  public static $modules = array('flexslider');

  /**
   * User with permission to admin flexslider.
   *
   * @var \Drupal\user\UserInterface
   */
  protected $adminUser;

  /**
   * User with permission to access administration pages.
   *
   * @var \Drupal\user\UserInterface
   */
  protected $anyUser;

  /**
   * {@inheritdoc}
   */
  public function setUp() {
    parent::setUp();

    // Create users.
    $this->adminUser = $this->drupalCreateUser(array('administer flexslider'), NULL, TRUE);
    $this->anyUser = $this->drupalCreateUser(array('access administration pages'));
  }

  /**
   * Admin Access test.
   */
  public function testAdminAccess() {

    // Login as the admin user.
    $this->drupalLogin($this->adminUser);

    // Load admin page.
    $this->drupalGet('admin/config/media/flexslider');
    $this->assertResponse(200, t('Administrative permission allows access to administration page.'));

    // Logout as admin user.
    $this->drupalLogout();

    // Login as any user.
    $this->drupalLogin($this->anyUser);

    // Attempt to load admin page.
    $this->drupalGet('admin/config/media/flexslider');
    $this->assertResponse(403, t('Regular users do not have access to administrative pages.'));

  }

  /**
   * Test managing the optionset.
   */
  public function testOptionSetCrud() {
    // Login as the admin user.
    $this->drupalLogin($this->adminUser);
    $testsets  = array('testset', 'testset2');

    foreach ($testsets as $name) {
      // Create a new optionset with default settings.
      /** @var Flexslider $optionset */
      $optionset = Flexslider::create(array('id' => $name, 'label' => $name));
      $this->assertTrue($optionset->id() == $name, t('Optionset object created: @name', array('@name' => $optionset->id())));
      $this->assertFalse(empty($optionset->getOptions()), t('Create optionset works.'));

      // Save the optionset to the database.
      $optionset->save();

      $this->assertFalse(FALSE === $optionset, t('Optionset saved to database.'));

      // Read the values from the database.
      $optionset = Flexslider::load($name);

      $this->assertTrue(is_object($optionset), t('Loaded option set.'));
      $this->assertEqual($name, $optionset->id(), t('Loaded name matches: @name', array('@name' => $optionset->id())));

      /** @var Flexslider $default_optionset */
      $default_optionset = Flexslider::create();
      foreach ($default_optionset->getOptions() as $key => $value) {
        $this->assertEqual($value, $optionset->getOptions()[$key], t('Option @option matches saved value.', array('@option' => $key)));
      }

    }

    // Load all optionsets.
    $optionsets = Flexslider::loadMultiple();
    $this->assertTrue(is_array($optionsets), t('Array of optionsets loaded'));
    $this->assertTrue(count($optionsets) == 3, t('Proper number of optionsets loaded (two created, one default): 3'));

    // Ensure they all loaded correctly.
    foreach ($optionsets as $optionset) {
      $this->assertTrue($optionset->id(), t('Loaded optionsets have a defined machine name'));
      $this->assertTrue($optionset->label(), t('Loaded optionsets have a defined human readable name (label)'));
      $this->assertTrue(!empty($optionset->getOptions()), t('Loaded optionsets have a defined array of options'));
    }

    // Update the optionset.
    $test_options = $this->getTestOptions();
    $test_options = $test_options['valid'];

    // Load one of the test option sets.
    $optionset = Flexslider::load($testsets[0]);

    // Change the settings.
    $optionset->setOptions($test_options['set2'] + $optionset->getOptions());

    // Save the updated values.
    $saved = $optionset->save();

    $this->assertTrue(SAVED_UPDATED == $saved, t('Saved updates to optionset to database.'));

    // Load the values from the database again.
    $optionset = Flexslider::load($testsets[0]);

    // Compare settings to the test options.
    foreach ($test_options['set2'] as $key => $value) {
      $this->assertEqual($optionset->getOptions()[$key], $value, t('Saved value matches set value: @key', array('@key' => $key)));
    }

    // Delete the optionset.
    $this->assertTrue(is_object($optionset), t('Optionset exists and is ready to be deleted.'));
    try {
      $optionset->delete();
      // Ensure the delete is successful.
      $this->pass(t('Optionset successfully deleted: @name', array('@name' => $optionset->id())));
    }
    catch (\Exception $e) {
      $this->fail(t('Caught exception: @msg', array('@msg' => $e->getMessage())));
    }

  }

  /**
   * Test the option set form.
   */
  public function testOptionSetForm() {

    // Login with admin user.
    $this->drupalLogin($this->adminUser);

    // ------------ Test Option Set Add ------------ //
    // Load create form.
    $this->drupalGet('admin/config/media/flexslider/add');
    $this->assertResponse(200, t('Administrative user can reach the "Add" form.'));

    // Save new optionset.
    $optionset = array();
    $optionset['label'] = t('testset');
    $optionset['id'] = 'testset';
    $this->drupalPostForm('admin/config/media/flexslider/add', $optionset, t('Save'));

    $this->assertResponse(200);
    $this->assertText('Created the testset FlexSlider optionset.', t('Successfully saved the new optionset "testset"'));

    // Attempt to save option set of the same name again.
    $this->drupalPostForm('admin/config/media/flexslider/add', $optionset, t('Save'));
    $this->assertResponse(200);
    $this->assertText('The machine-readable name is already in use. It must be unique.', t('Blocked the creation of duplicate named optionset.'));

    // ------------ Test Option Set Edit ------------ //
    // Attempt to save each option value.
    $options = $this->getTestOptions();

    foreach ($options['valid'] as $testset) {
      $this->drupalPostForm('admin/config/media/flexslider/default', $testset, t('Save'));
      $this->assertResponse(200);

      // Test saved values loaded into form.
      $this->drupalGet('admin/config/media/flexslider/default');
      $this->assertResponse(200, t('Default optionset reloaded.'));
      foreach ($testset as $key => $option) {
        $this->assertFieldByName($key, $option, t('Value for @key appears in form correctly.', array('@key' => $key)));
      }
    }

    // ------------ Test Option Set Delete ------------ //.
    $testset = Flexslider::load('testset');

    // Test the delete workflow.
    $this->drupalGet("admin/config/media/flexslider/{$testset->id()}/delete");
    $this->assertResponse(200);
    $this->assertText("Are you sure you want to delete {$testset->label()}?", t('Delete confirmation form loaded.'));
    $this->drupalPostForm("admin/config/media/flexslider/{$testset->id()}/delete", [], t('Delete'));
    $this->assertResponse(200);
    $this->assertText("Deleted the {$testset->label()} FlexSlider optionset.", t('Deleted test set using form.'));

  }

  /**
   * Get the test configuration options.
   *
   * @return array
   *   Returns an array of options to test saving.
   */
  protected function getTestOptions() {
    // Valid option set data.
    $valid = array(
      'set1' => FlexsliderDefaults::defaultOptions(),
      'set2' => array(
        'animation' => 'slide',
        'startAt' => 4,
        // @todo add more option tests
      ),
    );

    // Invalid edge cases.
    $error = array();

    return array('valid' => $valid, 'error' => $error);
  }

}
