try:
    from ._formatter import format_text as format_text_cpp
    _use_cpp = True
except ImportError:
    _use_cpp = False

def format_text_py(text):
    if not text or not isinstance(text, str):
        return text
    
    format_map = {
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
    }
    
    if '|' in text and (text.startswith('l|') or text.startswith('link|') or text.startswith('a|')):
        parts = text.split('|', 2)
        if len(parts) >= 3:
            link_text, url = parts[1].strip(), parts[2].strip()
            return f'<a href="{url}">{link_text}</a>'
        elif len(parts) == 2:
            url = parts[1].strip()
            return f'<a href="{url}">{url}</a>'
    
    if '|' in text:
        format_part, content = text.split('|', 1)
    elif ' ' in text:
        parts = text.split(' ', 1)
        format_part, content = parts[0], parts[1]
    else:
        return text
    
    format_list = []
    
    if '-' in format_part:
        format_list = format_part.split('-')
    elif '+' in format_part:
        format_list = format_part.split('+')
    elif '.' in format_part:
        format_list = format_part.split('.')
    elif ',' in format_part:
        format_list = format_part.split(',')
    elif '_' in format_part:
        format_list = format_part.split('_')
    else:
        format_list = [format_part]
    
    valid_formats = []
    for fmt in format_list:
        fmt = fmt.strip().lower()
        if fmt in format_map:
            valid_formats.append(format_map[fmt])
    
    if not valid_formats:
        return text
    
    opening_tags = []
    closing_tags = []
    
    for fmt in valid_formats:
        if fmt == 'tg-spoiler':
            opening_tags.append('<span class="tg-spoiler">')
            closing_tags.insert(0, '</span>')
        else:
            opening_tags.append(f'<{fmt}>')
            closing_tags.insert(0, f'</{fmt}>')
    
    return ''.join(opening_tags) + content + ''.join(closing_tags)

def format_text(text):
    if _use_cpp:
        return format_text_cpp(text)
    return format_text_py(text)

def format(text):
    return format_text(text)

def f(text):
    return format_text(text)

def fmt(text):
    return format_text(text)
