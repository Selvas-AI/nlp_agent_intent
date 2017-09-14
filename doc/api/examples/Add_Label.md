**Add Label**
----
  Adds a labeled example to the specified application

* **URL**

  /api/v1.0/apps/{app id}/example

* **Method:**

  `POST`

* **Data Params**

  *  ExampleText 필드에 입력 문장을 적는다. (필수)
  *  IntentName 필드에 의도를 적는다. (선택)
  
```
json
{
  "ExampleText" : string,
  "IntentName" : string
}
```

* **Success Response:**
  
  example 에 대한 id를 반환 한다.

  * **Code:** 200 <br />
    **Content:** `id`
        
 
* **Error Response:**

  * **Code:** 400 <br />
