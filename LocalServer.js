const http  = require('http');
const url   = require('url');
const fs    = require('fs');
const path  = require('path');
const port  = parseInt(process.argv[2] || 4242, 10);

// maps file extention to MIME typere
const formatsMap = new Map([
    [ '.ico',   'image/x-icon'          ],
    [ '.html',  'text/html'             ],
    [ '.js',    'text/javascript'       ],
    [ '.wasm',  'application/wasm'      ],
]);

const onFileRequest = async (req, res) => {

    console.log(`${req.method} ${req.url}`);

    const parsedUrl = url.parse(req.url);
    let pathname = `.${parsedUrl.pathname}`;
    const ext = path.parse(pathname).ext;

    if (!fs.existsSync(pathname)) {
        // if the file is not found, return 404
        res.statusCode = 404;
        res.end(`File "${pathname}" not found !`);
        return;
    }

    if (fs.statSync(pathname).isDirectory()) {
        res.statusCode = 400;
        res.end(`File "${pathname}" is a directory !`);
        return;
    }

    let data = fs.readFileSync(pathname);
    let contentType = "text/plain";

    // if the file is found, set Content-type and send data
    res.setHeader('Content-type', formatsMap.get(ext) || contentType );

    // firefox mulithreading
    res.setHeader('Cross-Origin-Opener-Policy', "same-origin");
    res.setHeader('Cross-Origin-Embedder-Policy', "require-corp");


    res.end(data);
}

const onListen = () =>  {

    console.log(`Server listening`);
    console.log(`=> http://127.0.0.1:${port}/`);
}

http.createServer(onFileRequest).listen(port, onListen);

