**Add Label**
----
  Adds a labeled example to the specified application

* **URL**

  /intent/example

* **Method:**

  `POST`

* **Data Params**

  *  ExampleText 필드에 입력 문장을 적는다. (필수)
  *  IntentName 필드에 의도를 적는다. (필수)
  
```
json
{
  "ExampleText" : string,
  "IntentName" : string
}
```

* **Success Response:**

  * **Code:** 200 <br />
         
* **Error Response:**

  * **Code:** 400 <br />
