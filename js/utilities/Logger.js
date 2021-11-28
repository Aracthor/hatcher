
class Logger {

    constructor(textAreaElementId) {

        this._textAreaElement = document.querySelector(textAreaElementId);

        if (!this._textAreaElement)
            throw new Error(`DOM elements not found, id=${textAreaElementId}`);

        this._lines = [];
        this._maxLines = 30;

        this._textAreaElement.value = ""; // <= clear any browser cache
    }

    log(text) {

        console.log(text);

        this._pushText(text);
    }

    error(text) {

        console.error(text);

        this._pushText(`[ERR] - ${text}`);
    }

    _pushText(text) {

        this._lines.push(text);
        if (this._lines.length > this._maxLines)
            this._lines.splice(0, this._lines.length - this._maxLines);

        this._textAreaElement.value = `${this._lines.join("\n")}\n`;

        // force focus on last line
        this._textAreaElement.scrollTop = this._textAreaElement.scrollHeight;
    }
}

export default Logger;
