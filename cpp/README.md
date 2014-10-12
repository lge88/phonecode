# Notes

- `vector::pop_back()` does not return anything. Use `vector::back()`
  to get the last element in a vector.

- The syntax for pointers to instance method:

```
// Type definition:
typedef string (ClassNmae::*MyFuncType)(string);

// Declare & init:
MyFuncType myfunc = &ClassName::methodName;

// Invoke:
(obj.*myfunc)(args)
```

- Concat (works for both string and vector):
```
vec1.insert(vec1.end(), vec2.begin(), vec2.end())
```

- Test whether a key is in a dictionary:
```
dict.find(key) == dict.end()
```
