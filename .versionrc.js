// This file is managed by dropfort/dropfort_module_build.
// Modifications to this file will be overwritten by default.

const fs = require("fs");

let infoFile;

try {
  infoFile =
    fs
      .readdirSync(__dirname)
      .filter(file => file.match(/[\s\S]+.info.yml/))[0] || null;
} catch (error) {
  infoFile = null;
}

const config = {
  types: [
    {
      type: "feat",
      section: "Features",
      hidden: false,
    },
    {
      type: "fix",
      section: "Bug Fixes",
      hidden: false,
    },
    {
      type: "perf",
      section: "Performance Improvements",
      hidden: false,
    },
    {
      type: "revert",
      section: "Reverts",
      hidden: false,
    },
    {
      type: "docs",
      section: "Documentation",
      hidden: false,
    },
    {
      type: "style",
      section: "Styles",
      hidden: true,
    },
    {
      type: "chore",
      section: "Miscellaneous Chores",
      hidden: true,
    },
    {
      type: "refactor",
      section: "Code Refactoring",
      hidden: false,
    },
    {
      type: "test",
      section: "Tests",
      hidden: true,
    },
    {
      type: "build",
      section: "Build System",
      hidden: false,
    },
    {
      type: "ci",
      section: "Continuous Integration",
      hidden: false,
    },
  ],
};

config.packageFiles = ["package.json"];

config.bumpFiles = [
  ...config.packageFiles,
  "package-lock.json",
  {
    filename: "composer.json",
    type: "json",
  },
  {
    filename: "composer.lock",
    type: "json",
  },
];

if (infoFile) {
  config.bumpFiles.push({
    filename: infoFile,
    updater: require.resolve("standard-version-updater-yaml"),
  });
}

config.scripts = {
  postbump: "composer update --lock",
};

module.exports = config;
