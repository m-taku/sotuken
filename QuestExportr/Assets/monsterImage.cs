using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class monsterImage : MonoBehaviour
{
    [SerializeField] Sprite[] m_imagi = { null};
    public monster m_monster = monster.monster1;
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        this.GetComponent<Image>().sprite = m_imagi[m_monster.GetHashCode()];
    }
}