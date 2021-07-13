
"use strict"

import Logger from "./utilities/Logger.js";

const onGlobalPageLoad = async () => {

    const logger = new Logger("loggerOutput");

    logger.log("[JS] page loaded");

    const onBasicGlobalPageError = (event) => {
        logger.error(`[JS] exception, event=${event.message}`);
    };
    window.addEventListener("error", onBasicGlobalPageError);

    const errorText = document.querySelector("#errorText");
    const renderArea = document.querySelector("#renderArea");
    const canvas = document.querySelector("#emscriptenCanvas");

    const showErrorText = (htmlText) => {
        canvas.style.display = "none"; // hide
        errorText.innerHTML = htmlText;
        errorText.style.display = "block"; // show
    };
    const showCanvas = () => {
        errorText.style.display = "none"; // hide
        canvas.style.display = "block"; // show
    };


    const onContextCreationError = (event) => {

        event.preventDefault();

        const statusMessage = event.statusMessage || "Unknown error";
        logger.error(`[JS] could not create a WebGL context, statusMessage="${statusMessage}".`);
    };
    canvas.addEventListener("webglcontextcreationerror", onContextCreationError, false);

    const onWebGlContextLost = (event) => {

        event.preventDefault();

        logger.error("[JS] WebGL context lost. You will need to reload the page.");
    };
    canvas.addEventListener("webglcontextlost", onWebGlContextLost, false);

    // this prevent the contextual menu to appear on a right click
    const onContextMenu = (event) => {
        event.preventDefault();
    };
    canvas.addEventListener("contextmenu", onContextMenu, false);

    let webglCtx;

    try {

        if (!window.Worker)
            throw new Error(`missing WebWorker feature`);

        logger.log("[JS] WebWorker feature => supported");


        const wasmSupported = (() => {
            try {
                if (typeof(WebAssembly) === "object" && typeof(WebAssembly.instantiate) === "function") {

                    const module = new WebAssembly.Module(Uint8Array.of(0x0, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00));

                    if (module instanceof WebAssembly.Module)
                        return new WebAssembly.Instance(module) instanceof WebAssembly.Instance;
                }
            } catch (err) {
                logger.error("[JS] Cannot instanciate WebAssembly module.");
            }
            return false;
        })();

        if (!wasmSupported)
            throw new Error("missing WebAssembly feature (unsuported)");

        logger.log("[JS] WebAssembly feature => supported");


        if (!window.WebGLRenderingContext)
            throw new Error("missing WebGL feature (unsuported)");

        logger.log("[JS] WebGL feature => supported");

        const renderingContextAttribs = {
            alpha: false,
            antialias: false,
            depth: true,
            failIfMajorPerformanceCaveat: false,
            premultipliedAlpha: true,
            preserveDrawingBuffer: true,
            stencil: false,

            // A hint to the user agent indicating what configuration of GPU is
            // suitable for the WebGL context. Possible values are:
            // "default":
            //     Let the user agent decide which GPU configuration is most
            //     suitable. This is the default value.
            // "high-performance":
            //     Prioritizes rendering performance over power consumption.
            // "low-power":
            //     Prioritizes power saving over rendering performance.
            powerPreference: "high-performance",
        };

        webglCtx = canvas.getContext("webgl2", renderingContextAttribs);

        if (!webglCtx)
            throw new Error("WebGL context failed (initialisation)");

        logger.log("[JS] WebGL context => initialised");
    }
    catch (err) {

        logger.error(`[JS] dependencies check failed: message="${err.message}"`);
        return;
    }

    const Module = {
        print: (text) => { logger.log(`[C++] ${text}`); },
        printErr: (text) => { logger.error(`[C++] ${text}`); },

        canvas: canvas,
        preinitializedWebGLContext: webglCtx,
        noExitRuntime: true,
        arguments: [
        ],
    };

    // this is needed by the wasm side
    window.Module = Module;

    logger.log("[JS] Downloading main script...")
    try {

        const scriptLoadingUtility = (src) => {
            return new Promise((resolve, reject) => {
                const scriptElement = document.createElement("script");
                scriptElement.src = src;
                scriptElement.onprogress = (event) => logger.log("event", event);
                scriptElement.onload = resolve;
                scriptElement.onerror = reject;
                document.head.appendChild(scriptElement);
            });
        };

        await scriptLoadingUtility(`./js/bin/exec_release.js`)

        logger.log("[JS] wasm script: loading successful");

        showCanvas();
    }
    catch (err) {

        logger.error(`[JS] wasm script: loading failed, err=${err.message}`);

        showErrorText("Fatal error<br>consult logging for more information.");
    }
};

window.addEventListener("load", onGlobalPageLoad);
