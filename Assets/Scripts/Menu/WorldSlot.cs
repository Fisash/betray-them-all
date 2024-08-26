using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using System.IO;

public class WorldSlot : MonoBehaviour
{
    public TextMeshProUGUI nameText;
    public TextMeshProUGUI infoText;

    string pathToMe;

    SaveData save;

    public void UpdateDisplay(SaveData data)
    {
        save = data;
        nameText.text = save.name;
        //infoText.text = "Мир создан:\n<color=white>" + save.creationTime + "</color>";
        infoText.text = "Последний запуск:\n<color=white>" + save.lastLaunchTime + "</color>";

        pathToMe = PlayerPrefs.GetString("savePath") + "/" + save.name + ".dat";
    }

    public void TapPlayButton()
    {
        PlayerPrefs.SetString("loadPath", pathToMe);
        GameObject.Find("Canvas").GetComponent<ScenChanger>().StartOpenScen("Game");
    }

    public void TapRemoveButton()
    {
        if (File.Exists(pathToMe))
        {
            File.Delete(pathToMe);
        }
        Destroy(this.gameObject); 
    }
}
