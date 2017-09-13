**Create Intent Classifier**
----
  Adds an intent classifier to an application

* **URL**

  /api/v1.0/apps/{app id}/intents

* **Method:**

  `POST`

* **Data Params**

  * model 필드에 module 이름을 적어 주시면 됩니다.
  * 현재는 fasttext 가능 합니다.
  * app 생성 초기에 이미 fasttext module이 생성 되어 있기 때문에 module을 삭제 후 호출 하지 않으면 현재는 실패 합니다.  
``` json
{
  "model" : string
}
```

* **Success Response:**
  
  module 에 대한 id를 반환 한다.

  * **Code:** 200 <br />
    **Content:** `id`
        
 
* **Error Response:**

  * **Code:** 400 <br />
