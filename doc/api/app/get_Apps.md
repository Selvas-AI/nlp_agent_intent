**get Apps**
----
  get list of user's all applications with their metadata
        Metadata : {ID, Name, Description, Culture, Active, PublishDate, URL, AuthKey ,CreatedDate, ModifiedDate, NumberOfIntents, NumberOfEntities, IsTrained}

* **URL**

  /api/v1.0/apps

* **Method:**

  `GET`


* **Success Response:**
  
  app 에 대한 정보를 반환 한다.

  * **Code:** 200 <br />
    **Content:** `{"app name" : "app id"}`
        
 
* **Error Response:**

  * **Code:** 400 <br />
