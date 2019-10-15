using System.Text;
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;

[Serializable]
public struct SaveData
{
    public string QuestTitle;
}

public class Export : MonoBehaviour
{
    public SaveData dete;
    static int No = 0;
    void Start()
    {
        string Path =  "/../../GameTemplate/Game/Assets/TestQeste/save" + No +".smqd";
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
