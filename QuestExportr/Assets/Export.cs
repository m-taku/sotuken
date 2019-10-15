using System.Text;
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine.UI;

[Serializable]
public struct SaveData
{
    public string QuestTitle;
    public int time;
    public int m_time;
}

public class Export : MonoBehaviour
{
    GameObject m_tex;
    public SaveData dete;
    //
    static int No = 0;
    private void Start()
    {
        m_tex = GameObject.Find("Text");
    }
    private void Update()
    { 
        m_tex.transform.GetComponent<Text>().text = dete.QuestTitle;
    }

    public void ExpoetQuest()
    {
        string Path = "/../../GameTemplate/Game/Assets/TestQeste/save" + No + ".smqd";
        string SavePath = Application.dataPath + Path;

        ///// 保存 /////////////////////////////////////
        {
            UnicodeEncoding uniencoding = new UnicodeEncoding();
            // byte[] bytes1 = BitConverter.GetBytes(dete.Id);
            char[] Name = new char[dete.QuestTitle.Length];
            Debug.Log(Name.Length);
            for (int i = 0; i < dete.QuestTitle.Length; i++)
            {
                Name[i] = dete.QuestTitle[i];
            }
            byte[] bytes = uniencoding.GetBytes(Name);
            byte[] bytes1 = BitConverter.GetBytes(bytes.Length);
            //byte[] bytes3 = BitConverter.GetBytes(dete.TemporaryData);
            // Create a file to write to.
            using (FileStream sw = File.Create(SavePath))
            {
                sw.Seek(0, SeekOrigin.End);
                Debug.Log(bytes.Length);
                sw.Write(bytes1, 0, bytes1.Length);
                sw.Write(bytes, 0, bytes.Length);
                // sw.Write(bytes3, 0, bytes3.Length);
                sw.Close();
            }
        }
        No++;
    }
}
