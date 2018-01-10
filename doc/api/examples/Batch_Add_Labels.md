**Batch Add Labels**
----
  Adds a batch of labeled examples to the specified application

* **URL**

  /intent/examples

* **Method:**

  `POST`

* **Data Params**

  *  ExampleText 필드에 입력 문장을 적는다. (필수)
  *  IntentName 필드에 의도를 적는다. (필수)
  
``` json
{
  "utterances" : [ {
    "ExampleText" : string,
    "IntentName" : string
  }, ...
  ]
}
```

* **Success Response:** 

  * **Code:** 200 <br />
        
 
* **Error Response:**

  * **Code:** 400 <br />
