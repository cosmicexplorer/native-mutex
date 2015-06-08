{
  "targets": [
    {
      "target_name": "native-mutex",
      "sources": [
        "native-mutex.cpp",
        "mutex.cpp",
        "condition-variable.cpp"
                 ],
      "conditions": [
        [
          'OS!="win"', {
            'cflags': [
              '-std=c++14'
            ]
          }
        ]
      ]
    }
  ]
}
