using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Camera : MonoBehaviour
{

    public Transform cameraTransform;
    public float CamSpeed;

    public float movTime;

    private Vector3 newP;
    private Quaternion rotP;
    public float rotAm;
    public Vector3 zoom;
    private Vector3 newZoom;
    public Vector3 dragSt;
    public Vector3 dragM;
    

    // Start is called before the first frame update
    void Start()
    {
        newP = transform.position;
        rotP = transform.rotation;
        newZoom = cameraTransform.localPosition;
        

    }

    // Update is called once per frame
    void Update()
    {
        controlMov();
    }

    void controlMov()
    {
        if (Input.GetKey(KeyCode.W))
        {
            newP += (transform.forward*CamSpeed); 
        }
        if (Input.GetKey(KeyCode.S))
        {
            newP += (transform.forward*-CamSpeed); 
        }  if (Input.GetKey(KeyCode.D))
        {
            newP += (transform.right*CamSpeed); 
        }
        if (Input.GetKey(KeyCode.A))
        {
            newP += (transform.right*-CamSpeed); 
        }
        if (Input.GetKey(KeyCode.Q))
        {
            rotP *=(Quaternion.Euler(Vector3.up*-rotAm));
        }
        if (Input.GetKey(KeyCode.E))
        {
            rotP *=(Quaternion.Euler(Vector3.up*rotAm));
        }

        if (Input.mouseScrollDelta.y>0)
        {
            newZoom += zoom;
            
        }
        if (Input.mouseScrollDelta.y<0)
        {
            newZoom -= zoom;
            
        }
        transform.position = Vector3.Lerp(transform.position, newP, Time.deltaTime * movTime);
        transform.rotation = Quaternion.Lerp(transform.rotation, rotP, Time.deltaTime * movTime);
        cameraTransform.localPosition = Vector3.Lerp(cameraTransform.localPosition, newZoom, Time.deltaTime * movTime);

    }


}
