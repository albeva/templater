Purpose
=======

A language independent and simple template engine is useful for applications that
use code which is written in more than one programming language.

Inspiration
===========

This is *heavily* inspired by Twig Engine, but simplified and made fit my purposes.

Syntax
======

Basic syntax elements:

    void example() {
        {# This is a comment #}
        auto title = ${ title | quoted };
        {% if showGreeting %}
            {% for greeting in greetings %}
                std::cout {%
                    if !loop.first %}std::cout << ", "{% endif
                %} << ${ greeting | quoted }{%
                    if loop.last %}std::cout << '\n';{% endif
                %};
            {% endfor %}
        {% endif %}
    }

Comments and Whitespace
-----------------------

Everything between ``{#`` and ``#}`` is ignored by the lexer.

Expressions
-----------

Expressions allow basic logical operations. Operators and their precedence matches C++ rules.

    =========== ==============================================================
    Operator    Description
    =========== ==============================================================
    ``!``       Unary logical NOT.
    ``<``       Less than
    ``<=``      Less or equal than
    ``>``       Greater than
    ``>=``      Greater or equal than
    ``==``      Equal to
    ``!=``      Not equal to
    ``&&``      Logical and
    ``||``      Logical or
    =========== ==============================================================

Use parentheses to group expressions.

Types
-----

The following types exist.

    =========== ==============================================
    Type        Description
    =========== ==============================================
    ``number``  Numeric integral, treated as uint64_t. Engine
                parses only positive integer literals.
    ``string``  String literal
    ``array``   Is the primary input type derived from table
    =========== ==============================================

Property Lookup
---------------

Properties are namespaced using `.`:

    foo.name 

Arrays can be indexed either with `[<Number Literal>|variable]` or `.<Number Literal>`:
    
    ${ Names[0] }            {# yields first element #}
    ${ Names.0 }             {# yields first element #}

    ${ Names|reverse[0] }    {# yields last elment #}
    ${ Names|reverse.0 }     {# yields last elment #}

    ${ Names[for.revindex] } {# yields "reverse" item of the iteration #}

Filtering
---------

The template language does not specify function calls but filters can be used
to further modify variables using functions the template engine provides.

All underlying data is immutable. Applying a filter will produce new output,
but will not modify underlying dataset.

The following snippet shows how filters are translated to function calls::

    ${ 42 | foo | bar | baz }
        ->  baz(bar(foo(42)))

The following filters are provided by the implementation:

    =================== ======================================================
    Name                Description
    =================== ======================================================
    `quoted`            Add quotes and escape a string. This also converts
                        `"` to \".
    `title`             Capitalizes only the first char of the whole string.
    `upper`             Convert the string to uppercase
    `lower`             Convert the string to lowercase
    `join`              Concatenate the array items and join them with the
                        string provided or an empty string.
                        ${ Names | join ", " }
    `reverse`           Reverse the Array items
    `count`             Count the number of items in an array or string
                        characters.
    `orderby`           Orders array by given key
                        ${ Names | orderby name }
    `groupby`           Groups elements in array and produces a nested array
                        {% for groups in Tokens | groupby precedence %}
                            {% for token in groups  %}
                                ...
                            {% endfor %}
                        {% endfor %}
    `columns`           Yields array of table columns
    `select`            Selects a single property from an array
    `repeat`            Repeats input n times
                        ${ "-" | repeat 5 }
    =================== ======================================================

For Loops
---------

Iteration works via for loops. Inside loops you can query `for` for runtime information:

    ====================== ===================================================
    Variable               Description
    ====================== ===================================================
    ``for.index``          The current iteration of the loop
    ``for.revindex``       The number of iterations from the end of the
    ``for.first``          True if this is the first time through the loop
    ``for.last``           True if this is the last time through the loop
    ``for.count``          Number of loops to be performed in total
    ====================== ===================================================

If Conditions
-------------

If conditions work like Ruby, PHP and Python.

    {% if expr1 %}
        ...
    {% elseif expr2 %}
        ...
    {% else %}
        ...
    {% endif %}
