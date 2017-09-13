**get Trained Model Prediction**
----
  gets the trained model predictions for the input example

* **URL**

  /api/v1.0/apps/{app id}/predict

* **Method:**

  `GET`

*  **URL Params**

   **Required:**
 
   `example=[string]`

* **Success Response:**
  
  의도에 대한 이름을 반환한다.

  * **Code:** 200 <br />
    **Content:** `intent`
        
 
* **Error Response:**

  * **Code:** 400 <br />
