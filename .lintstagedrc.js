// This file is managed by dropfort/dropfort_module_build.
// Modifications to this file will be overwritten by default.

module.exports = {
  "*.js": "eslint --fix",
  "*.{scss,sass}": "stylelint --fix --allow-empty-input",
  "*.{php,module,inc,install,test,profile,theme}":
    "phpcbf --standard=Drupal,DrupalPractice --extensions=php,module,inc,install,test,profile,theme",
};
