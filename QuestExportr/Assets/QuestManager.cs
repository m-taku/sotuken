using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Linq;
namespace oyako
{
    public static class ChildrenSearch
    {
        public static GameObject[] GetChildren(GameObject parentName)
        {
            // 子のTransform[]を取り出す
            Transform[] transforms = parentName.GetComponentsInChildren<Transform>(true);
            // 使いやすいようにtransformsからgameObjectを取り出す
            var gameObjects = from t in transforms select t.gameObject;
            // 配列に変換してreturn
            return gameObjects.ToArray();
        }
    }
}

public class QuestManager : MonoBehaviour
{
    int No = 1;
    bool m_export = false;
    GameObject[] m_Quest;
    private IEnumerator coroutine;
    // Start is called before the first frame update
    void Start()
    {
        m_Quest = oyako.ChildrenSearch.GetChildren(this.gameObject);
        m_Quest[No].SetActive(true);
        Debug.Log(m_Quest[0]);
    }

    // Update is called once per frame
    void Update()
    {
        if (!m_export)
        {
            if(Input.GetKeyDown(KeyCode.KeypadEnter))
            {
                coroutine = ExportQuest(m_Quest);

                Debug.Log("dadadadadadaddad");
                StartCoroutine(coroutine);
                m_export = true;
            }
            else if (Input.GetKeyDown(KeyCode.RightShift))
            {
                m_Quest[No].SetActive(false);
                No = Mathf.Min(No + 1, m_Quest.Length-1);
                m_Quest[No].SetActive(true);
                Debug.Log(No);
            }
            else if (Input.GetKeyDown(KeyCode.LeftShift))
            {
                m_Quest[No].SetActive(false);
                No = Mathf.Max(No - 1, 1);
                m_Quest[No].SetActive(true);
                Debug.Log(No);
            }

        }
    }



    private IEnumerator ExportQuest(GameObject[] obj)
    {

        for(int i=1;i< obj.Length;i++)
        {
            obj[i].GetComponent<Export>().ExpoetQuest();
        }
        yield break;
    }

}
