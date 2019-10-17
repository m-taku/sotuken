using System.Text;
using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine.UI;
using System.Linq;

public enum monster
{
    monster1,           //モンスターの名前をお願いします。
    monster2,
    monster3,
    num
}

[Serializable]
public struct SaveData
{
    public string QuestTitle;
    public monster m_monster;
    public int Remuneration;
    public int Timelimit;
    public int Failure;
    public string humanname;
    public string Remarks;
}

public class Export : MonoBehaviour
{
    /* byte配列→string */
    static string BytesToString(byte[] bytes)
    {
        return new string(bytes.Select(x => (char)x).ToArray());
    }
    /* string→byte配列 */
    static byte[] StringToBytes(string str)
    {
        return str.Select(x => (byte)x).ToArray();
    }
    GameObject[] m_Quest = { null };
    public SaveData dete = new SaveData()
    {
        QuestTitle = "oni",
        m_monster = monster.monster1,
        Remuneration = 20,
        Timelimit = 60,
        Failure = 3,
        humanname = "宮崎",
        Remarks = "あいえーーーーーーーーーー"
    };
    //
    static int No = 0;
    private void Start()
    {
       m_Quest = oyako.ChildrenSearch.GetChildren(GameObject.Find("Quest"));
    }
    private void Update()
    {
        m_Quest[2].GetComponent<Text>().text = dete.QuestTitle;
        m_Quest[3].GetComponent<monsterImage>().m_monster = dete.m_monster;
        m_Quest[4].GetComponent<Text>().text = dete.m_monster.ToString() + "の討伐";
        m_Quest[5].GetComponent<Text>().text = dete.Remuneration + "G";
        m_Quest[6].GetComponent<Text>().text = dete.Timelimit + "分";
        m_Quest[7].GetComponent<Text>().text = dete.Failure + "ダウン";
        m_Quest[8].GetComponent<Text>().text = dete.humanname;
        m_Quest[9].GetComponent<Text>().text = dete.Remarks;
        Debug.Log(dete.QuestTitle.Length);
    }

    public void ExpoetQuest()
    {
        string Path = "/../../GameTemplate/Game/Assets/TestQeste/save" + No + ".smqd";
        string SavePath = Application.dataPath + Path;

        ///// 保存 /////////////////////////////////////
        {
            UnicodeEncoding uniencoding = new UnicodeEncoding();
            // byte[] bytes1 = BitConverter.GetBytes(dete.Id);
            char[] Name = dete.QuestTitle.ToCharArray();
            byte[][] bytes = new byte[10][];
            //データがShift-JISの場合
            dete.QuestTitle += "\0";
            byte[] data = System.Text.Encoding.GetEncoding("shift_jis").GetBytes(dete.QuestTitle);
            bytes[0] = BitConverter.GetBytes(data.Length);
            bytes[1] = data;

            bytes[2] = BitConverter.GetBytes(dete.m_monster.GetHashCode());

            bytes[3] = BitConverter.GetBytes(dete.Remuneration);

            bytes[4] = BitConverter.GetBytes(dete.Timelimit);

            bytes[5] = BitConverter.GetBytes(dete.Failure);

            dete.humanname += "\0";
            byte[] data2 = System.Text.Encoding.GetEncoding("shift_jis").GetBytes(dete.humanname);
            bytes[6] = BitConverter.GetBytes(data2.Length);
            bytes[7] = data2;

            dete.Remarks += "\0";
            byte[] data3 = System.Text.Encoding.GetEncoding("shift_jis").GetBytes(dete.Remarks);
            bytes[8] = BitConverter.GetBytes(data3.Length);
            bytes[9] = data3;
            using (FileStream sw = File.Create(SavePath))
            {
                
                sw.Seek(0, SeekOrigin.End);
                for (int i = 0; i < bytes.Length; i++)
                {
                    sw.Write(bytes[i], 0, bytes[i].Length);
                }
                //sw.Write(bytes3, 0, bytes3.Length);
                sw.Close();
            }
        
            Debug.Log(bytes[1][0]);
            Debug.Log(bytes[1][1]);
            Debug.Log(bytes[1][2]);
            Debug.Log(bytes[1][3]);
        }
        No++;
    }
}
