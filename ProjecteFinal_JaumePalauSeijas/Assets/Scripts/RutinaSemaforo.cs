using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RutinaSemaforo : MonoBehaviour
{
    public Light red;

    public Light ambar;

    public Light verd;

    private bool a;
    // Start is called before the first frame update
    

    void Start()
    {
        
        a = true;
    }

    // Update is called once per frame
    void Update()
    {
        if (a)
        {
            StartCoroutine(semafor());
        }
    }

    IEnumerator semafor()
    {
        a = false;
        red.enabled = true;

        yield return new WaitForSeconds(2f);
        red.enabled = false;
        ambar.enabled = true;
        verd.enabled = false;
        yield return new WaitForSeconds(2f);
        red.enabled = false;
        ambar.enabled = false;
        verd.enabled = true;
        yield return new WaitForSeconds(2f);
        red.enabled = true;
        ambar.enabled = false;
        verd.enabled = false;
        a = true;
    }
}
