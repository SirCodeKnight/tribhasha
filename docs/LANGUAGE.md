# त्रिभाषा (Tribhasha) Language Reference

This document provides a comprehensive reference for the त्रिभाषा (Tribhasha) programming language, covering syntax, semantics, and usage in all three supported languages: Assamese, Hindi, and English.

## Language Philosophy

त्रिभाषा is designed with the following principles in mind:

1. **Language Inclusivity**: Programming should be accessible to speakers of different languages.
2. **Syntactic Clarity**: Syntax should be clear and intuitive regardless of the language used.
3. **Seamless Integration**: Different languages should integrate seamlessly without conflicts.
4. **Performance**: The JIT compiler ensures near-native performance.

## Lexical Structure

### Keywords

| Concept | English | Hindi | Assamese |
|---------|---------|-------|----------|
| Variable | var | चर | ভেৰিয়েবল |
| Function | function | फलन | কাৰ্য্য |
| If | if | अगर | যদি |
| Else | else | अन्यथा | নহলে |
| While | while | जबतक | যতক্ষণ |
| For | for | के_लिए | ৰ_বাবে |
| Return | return | वापस | ঘূৰাই_দিয়ক |
| True | true | सही | সত্য |
| False | false | गलत | মিছা |
| And | and | और | আৰু |
| Or | or | या | বা |
| Not | not | नहीं | নহয় |

### Data Types

| Type | English | Hindi | Assamese |
|------|---------|-------|----------|
| Integer | int | पूर्णांक | পূৰ্ণসংখ্যা |
| Float | float | दशमलव | দশমিক |
| String | string | वाक्य | বাক্য |
| Boolean | bool | बूलियन | বুলিয়ান |
| Array | array | सरणी | এৰে |

## Grammar

The language follows a C-style syntax with adaptations for multilingual support:

### Variable Declaration

```
// English
var x = 10;

// Hindi
चर x = 10;

// Assamese
ভেৰিয়েবল x = 10;
```

### Function Declaration

```
// English
function add(a, b) {
    return a + b;
}

// Hindi
फलन जोड़(a, b) {
    वापस a + b;
}

// Assamese
কাৰ্য্য যোগ(a, b) {
    ঘূৰাই_দিয়ক a + b;
}
```

### Conditional Statements

```
// English
if (x > 10) {
    print("Greater");
} else {
    print("Lesser or equal");
}

// Hindi
अगर (x > 10) {
    दिखाओ("बड़ा है");
} अन्यथा {
    दिखाओ("छोटा या बराबर है");
}

// Assamese
যদি (x > 10) {
    দেখুৱাওক("ডাঙৰ");
} নহলে {
    দেখুৱাওক("সৰু বা সমান");
}
```

### Loops

```
// English - While Loop
while (i < 10) {
    print(i);
    i = i + 1;
}

// Hindi - For Loop
के_लिए (चर i = 0; i < 10; i = i + 1) {
    दिखाओ(i);
}

// Assamese - While Loop
যতক্ষণ (i < 10) {
    দেখুৱাওক(i);
    i = i + 1;
}
```

## Mixed Language Programming

त्रिभाषा uniquely allows mixing languages within the same file:

```tribhasha
फलन calculate_factorial(n) {
    যদি (n <= 1) {
        return 1;
    }
    वापस n * calculate_factorial(n - 1);
}

var result = calculate_factorial(5);
दिखाओ("The factorial is: " + result);
```

## Standard Library

त्रिभाषा includes a standard library with common functions in all three languages:

| Functionality | English | Hindi | Assamese |
|---------------|---------|-------|----------|
| Print to console | print() | दिखाओ() | দেখুৱাওক() |
| Get input | input() | इनपुट() | ইনপুট() |
| Length of array | length() | लंबाई() | দৈৰ্ঘ্য() |
| Convert to string | toString() | स्ट्रिंग_में() | স্ট্ৰিং_লৈ() |
| Convert to number | toNumber() | संख्या_में() | সংখ্যা_লৈ() |

## Type System

त्रिभाषा uses dynamic typing with type inference. Explicit type annotations are optional:

```
// Dynamic typing (all languages)
var x = 10;
x = "hello";  // Valid, x is now a string

// With type annotation (English)
int y = 20;
y = "world";  // Error

// With type annotation (Hindi)
पूर्णांक z = 30;
z = "नमस्ते";  // Error

// With type annotation (Assamese)
পূৰ্ণসংখ্যা w = 40;
w = "নমস্কাৰ";  // Error
```

## Error Handling

```
// English
try {
    // risky code
} catch (error) {
    print("Error: " + error);
}

// Hindi
कोशिश_करो {
    // risky code
} पकड़ो (त्रुटि) {
    दिखाओ("त्रुटि: " + त्रुटि);
}

// Assamese
চেষ্টা_কৰক {
    // risky code
} ধৰক (ত্ৰুটি) {
    দেখুৱাওক("ত্ৰুটি: " + ত্ৰুটি);
}
```

## Advanced Features

### Lambda Functions

```
// English
var add = (a, b) => a + b;

// Hindi
चर जोड़ = (a, b) => a + b;

// Assamese
ভেৰিয়েবল যোগ = (a, b) => a + b;
```

### Classes and Objects

```
// English
class Person {
    function constructor(name, age) {
        this.name = name;
        this.age = age;
    }
    
    function greet() {
        return "Hello, I am " + this.name;
    }
}

// Hindi
वर्ग व्यक्ति {
    फलन निर्माता(नाम, उम्र) {
        यह.नाम = नाम;
        यह.उम्र = उम्र;
    }
    
    फलन अभिवादन() {
        वापस "नमस्ते, मैं " + यह.नाम + " हूँ";
    }
}

// Assamese
শ্ৰেণী ব্যক্তি {
    কাৰ্য্য নিৰ্মাতা(নাম, বয়স) {
        এই.নাম = নাম;
        এই.বয়স = বয়স;
    }
    
    কাৰ্য্য অভিবাদন() {
        ঘূৰাই_দিয়ক "নমস্কাৰ, মোৰ নাম " + এই.নাম;
    }
}
```

## Performance Considerations

त्रिभाषा uses LLVM's ORC JIT compiler to achieve near-native performance. The compilation process:

1. Source code → Tokenization → Parsing → AST
2. AST → LLVM IR → Machine Code
3. JIT execution of the compiled code

This approach provides:
- Fast startup time
- Optimized runtime performance
- Cross-platform compatibility

## Best Practices

1. **Consistent Language Use**: Try to maintain consistent language within related blocks of code.
2. **Clear Naming**: Use descriptive names regardless of language.
3. **Comments**: Comment in any of the three languages or English for wider accessibility.
4. **Documentation**: Document APIs in all three languages when possible.

## Implementation Details

The त्रिभाषा compiler is implemented in C++ using LLVM's toolchain. The source code is available on [GitHub](https://github.com/SirCodeKnight/tribhasha).

---

This language reference will be expanded as the language evolves. For questions or suggestions, please open an issue on the GitHub repository.