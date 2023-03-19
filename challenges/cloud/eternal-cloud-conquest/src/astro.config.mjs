import { defineConfig } from "astro/config";
import tailwind from "@astrojs/tailwind";
import react from "@astrojs/react";
import remarkToc from "remark-toc";
import remarkCollapse from "remark-collapse";

import node from "@astrojs/node";

// https://astro.build/config
export default defineConfig({
  site: "https://astro-paper.pages.dev/",
  publicDir: "./src/public",
  integrations: [
    tailwind({
      config: {
        // applyBaseStyles: false,
      },
    }),
    react(),
  ],
  markdown: {
    remarkPlugins: [
      remarkToc,
      [
        remarkCollapse,
        {
          test: "Table of contents",
        },
      ],
    ],
    shikiConfig: {
      theme: "one-dark-pro",
      wrap: true,
    },
    extendDefaultPlugins: true,
  },
  vite: {
    optimizeDeps: {
      exclude: ["@resvg/resvg-js"],
    },
  },
  output: "server",
  adapter: node({
    mode: "standalone",
  }),
});
