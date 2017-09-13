**Add App**
----
  Creats a new application

* **URL**

  /api/v1.0/apps

* **Method:**

  `POST`

* **Data Params**

  Name 필드에 app 이름을 적어 주시면 됩니다.
``` json
{
  "Name" : string
}
```

* **Success Response:**
  
  app 에 대한 id를 반환 한다.

  * **Code:** 200 <br />
    **Content:** `id`
        
 
* **Error Response:**

  * **Code:** 400 <br />
