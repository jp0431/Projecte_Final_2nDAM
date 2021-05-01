using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Rutina : MonoBehaviour
{
    public Light llum;
    private bool man;

    private bool a;

    // Start is called before the first frame update
    public void Start()
    {
        a = true;
        MQTTClass.manF = true;
    }

    public void Update()
    {
        if (!MQTTClass.manF)
        {
            if (a)
            {
                StartCoroutine(rutina());
            }
            
        }
    
        // Update is called once per frame
        IEnumerator rutina()
        {
            a = false;
            llum.enabled = false;
            yield return new WaitForSeconds(5f);
            llum.enabled = true;
            yield return new WaitForSeconds(5f);
            a = true;

        }
    }
}
