let nativeFormatter;

try {
    nativeFormatter = require('../build/Release/pyformat');
} catch (e) {
    nativeFormatter = null;
}

const formatMap = {
    'b': 'b', 'bold': 'b',
    'i': 'i', 'italic': 'i',
    'u': 'u', 'underline': 'u',
    's': 's', 'strike': 's', 'del': 's',
    'c': 'code', 'code': 'code',
    'p': 'pre', 'pre': 'pre',
    'sp': 'tg-spoiler', 'spoiler': 'tg-spoiler',
    'mono': 'code', 'm': 'code',
    'em': 'i', 'emphasis': 'i',
    'strong': 'b', 'str': 'b',
    'mark': 'mark', 'highlight': 'mark',
    'small': 'small', 'sm': 'small',
    'big': 'big', 'large': 'big',
    'sub': 'sub', 'subscript': 'sub',
    'sup': 'sup', 'superscript': 'sup',
    'ins': 'ins', 'insert': 'ins',
    'kbd': 'kbd', 'key': 'kbd',
    'var': 'var', 'variable': 'var',
    'samp': 'samp', 'sample': 'samp',
    'tt': 'tt', 'teletype': 'tt',
    'cite': 'cite', 'citation': 'cite',
    'dfn': 'dfn', 'definition': 'dfn',
    'abbr': 'abbr', 'abbreviation': 'abbr',
    'q': 'q', 'quote': 'q'
};

function formatTextJS(text) {
    if (!text || typeof text !== 'string') return text;
    
    if (text.includes('|') && (text.startsWith('l|') || text.startsWith('link|') || text.startsWith('a|'))) {
        const parts = text.split('|');
        if (parts.length >= 3) {
            const linkText = parts[1].trim();
            const url = parts[2].trim();
            return `<a href="${url}">${linkText}</a>`;
        } else if (parts.length === 2) {
            const url = parts[1].trim();
            return `<a href="${url}">${url}</a>`;
        }
    }
    
    let formatPart, content;
    const pipeIndex = text.indexOf('|');
    const spaceIndex = text.indexOf(' ');
    
    if (pipeIndex !== -1) {
        formatPart = text.substring(0, pipeIndex);
        content = text.substring(pipeIndex + 1);
    } else if (spaceIndex !== -1) {
        formatPart = text.substring(0, spaceIndex);
        content = text.substring(spaceIndex + 1);
    } else {
        return text;
    }
    
    let formatList = [];
    
    if (formatPart.includes('-')) {
        formatList = formatPart.split('-');
    } else if (formatPart.includes('+')) {
        formatList = formatPart.split('+');
    } else if (formatPart.includes('.')) {
        formatList = formatPart.split('.');
    } else if (formatPart.includes(',')) {
        formatList = formatPart.split(',');
    } else if (formatPart.includes('_')) {
        formatList = formatPart.split('_');
    } else {
        formatList = [formatPart];
    }
    
    const validFormats = [];
    for (const fmt of formatList) {
        const lowerFmt = fmt.trim().toLowerCase();
        if (formatMap[lowerFmt]) {
            validFormats.push(formatMap[lowerFmt]);
        }
    }
    
    if (validFormats.length === 0) {
        return text;
    }
    
    let openingTags = '';
    let closingTags = '';
    
    for (const fmt of validFormats) {
        if (fmt === 'tg-spoiler') {
            openingTags += '<span class="tg-spoiler">';
            closingTags = '</span>' + closingTags;
        } else {
            openingTags += `<${fmt}>`;
            closingTags = `</${fmt}>` + closingTags;
        }
    }
    
    return openingTags + content + closingTags;
}

function formatText(text) {
    if (nativeFormatter) {
        return nativeFormatter.formatText(text);
    }
    return formatTextJS(text);
}

function format(text) {
    return formatText(text);
}

function f(text) {
    return formatText(text);
}

function fmt(text) {
    return formatText(text);
}

module.exports = {
    formatText,
    format,
    f,
    fmt
};
