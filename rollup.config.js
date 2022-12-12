import { babel } from "@rollup/plugin-babel";
import { terser } from "rollup-plugin-terser";
import cleanup from "rollup-plugin-cleanup";

const paths = {
  src: "./assets/js",
  dest: "./dist/js",
  files: ["flexslider.load"],
};

const defaultTerserOptions = {
  mangle: {
    reserved: ["Drupal"],
  },
};

export default args => {
  const sourcemap = args.sourcemap || false;

  return [
    ...paths.files.map(file => {
      return {
        input: `${paths.src}/${file}.js`,
        plugins: [babel({ babelHelpers: "inline" }), cleanup()],
        output: [
          {
            format: "iife",
            sourcemap,
            file: `${paths.dest}/${file}.js`,
          },
          {
            format: "iife",
            sourcemap,
            file: `${paths.dest}/${file}.min.js`,
            plugins: [terser(defaultTerserOptions)],
          },
        ],
      };
    }),
  ];
};
