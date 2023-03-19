Animals
=======

Defined animals in the input are:

    | ${ Animals | columns | select name | title | join " | " } |
    |{%
        for col in Animals | columns | select name %}-${
            "-" | repeat col.count }-{%
            if !for.first %}|{% endif %}{%
        endor
    %}|
    {% for animal in Animals %}| {%
            for col in animal %}${
                col }{%
                if !for.first %} | {% endif %}{%
            endfor
        %} |
    {% endfor %}
