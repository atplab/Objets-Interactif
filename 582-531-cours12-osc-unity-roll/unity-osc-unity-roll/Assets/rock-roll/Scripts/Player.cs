using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using extOSC;

public class Player : MonoBehaviour
{

    public extOSC.OSCReceiver oscReceiver;

    public float torqueForce = 1f;
    public float jumpForce = 5f;

    public float checkRadius = 0.1f;
    public LayerMask groundLayer;

    private Rigidbody2D rb;
    private int etatEnMemoire = 1;

        void TraiterOscAngle(OSCMessage message)
    {
        // Si le message n'a pas d'argument ou l'argument n'est pas un Int on l'ignore
        if (message.Values.Count == 0)
        {
            Debug.Log("No value in OSC message");
            return;
        }

        if (message.Values[0].Type != OSCValueType.Int)
        {
            Debug.Log("Value in message is not an Int");
            return;
        }

        int value = message.Values[0].IntValue;
        rb.AddTorque(torqueForce * -value);
    }

    void TraiterOscKey(OSCMessage message)

    {
        // Si le message n'a pas d'argument ou l'argument n'est pas un Int on l'ignore
        if (message.Values.Count == 0)
        {
            Debug.Log("No value in OSC message");
            return;
        }

        if (message.Values[0].Type != OSCValueType.Int)
        {
            Debug.Log("Value in message is not an Int");
            return;
        }

        int value = message.Values[0].IntValue;
       
        int nouveauEtat = value; // REMPLACER ici les ... par le code qui permet de r�cup�rer la nouvelle donn�e du flux
        if (etatEnMemoire != nouveauEtat)
        { // Le code compare le nouvel etat avec l'etat en m�moire
            etatEnMemoire = nouveauEtat; // Le code met � jour l'�tat m�moris�
            if (nouveauEtat == 0 && IsGrounded())
            {                    
                rb.AddForce(Vector2.up * jumpForce, ForceMode2D.Impulse);
            }
        }

     }



    void Start()
    {
        oscReceiver.Bind("/changeEncoder", TraiterOscAngle);
        oscReceiver.Bind("/boutonEncodeur", TraiterOscKey);
        rb = GetComponent<Rigidbody2D>();
    }

    void FixedUpdate()
    {
        // Roll left/right
        if (Input.GetKey(KeyCode.RightArrow))
        {
            rb.AddTorque(-torqueForce); // clockwise
        }
        if (Input.GetKey(KeyCode.LeftArrow))
        {
            rb.AddTorque(torqueForce); // counter-clockwise
        }

       
    }

    void Update()
    {
         // Jump
         // GetKeyDown() does not work in FixedUpdate()
        if (Input.GetKeyDown(KeyCode.UpArrow)) 
        {
            if (IsGrounded())
            {
                rb.AddForce(Vector2.up * jumpForce, ForceMode2D.Impulse);
            }
        }
    }

    public bool IsGrounded()
    {
        float extraHeight = 0.1f;
        RaycastHit2D hit = Physics2D.Raycast(
            transform.position,
            Vector2.down,
            GetComponent<CircleCollider2D>().radius + extraHeight,
            groundLayer
        );
        return hit.collider != null;
    }
}
