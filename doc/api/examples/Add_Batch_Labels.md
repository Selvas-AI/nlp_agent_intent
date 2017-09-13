**Add Batch Labels**
----
  Adds a batch of labeled examples to the specified application

* **URL**

  /api/v1.0/apps/{app id}/examples

* **Method:**

  `POST`

* **Data Params**

  *  ExampleText 필드에 입력 문장을 적는다. (필수)
  *  IntentName 필드에 의도를 적는다. (선택)
  
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
    **Content:** `Success`
        
 
* **Error Response:**

  * **Code:** 400 <br />
