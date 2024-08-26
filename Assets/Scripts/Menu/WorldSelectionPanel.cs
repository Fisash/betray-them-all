using System;
using UnityEngine;
using UnityEngine.UI;
using TMPro;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

public class WorldSelectionPanel : MonoBehaviour
{
    public GameObject worldToPlayPrefab;
    public Transform content;
    public GameObject choosePanel;
    public GameObject newWorldPanel;
    public TMP_InputField newWorldName;
    public TextMeshProUGUI newWorldButtonText;

    public List<SaveData> saves = new List<SaveData>();

    public ItemsAndClassesInfo info;

    public string savePath;

    public GameObject howToPlayPanel;
    public GameObject developersPanel;

    public void ChangeHowToPlayPanelActive(bool active) => howToPlayPanel.SetActive(active);
    public void ChangeDevelopersPanelActive(bool active) => developersPanel.SetActive(active);

    public void Start()
    {
        if (!PlayerPrefs.HasKey("savePath"))
        {
            PlayerPrefs.SetString("savePath", savePath);
        }
        savePath = PlayerPrefs.GetString("savePath");

        LoadWorlds();
    }
    public void LoadWorlds()
    {
        if (Directory.Exists(savePath))
        {
            string[] filePaths = Directory.GetFiles(savePath, "*.dat");
            BinaryFormatter formatter = new BinaryFormatter();

            foreach (string filePath in filePaths)
            {
                using (FileStream fs = new FileStream(filePath, FileMode.OpenOrCreate))
                {
                    if (fs.Length <= 0)
                    {
                        fs.Close();
                        File.Delete(filePath);
                        return;
                    }
                    SaveData data = (SaveData)formatter.Deserialize(fs);

                    GameObject loadedWorld = Instantiate(worldToPlayPrefab, content);
                    loadedWorld.GetComponent<WorldSlot>().UpdateDisplay(data);

                    saves.Add(data);
                }
            }

        }
    }
    public void ShowChoosePanel()
    {
        choosePanel.SetActive(true);
    }
    public void TapCancelButton()
    {
        if (newWorldPanel.activeSelf)
        {
            newWorldButtonText.text = "Создать новый мир";
            newWorldPanel.SetActive(false);
        }
        else
        {
            choosePanel.SetActive(false);
        }
    }
    public void TapNewWorldButton()
    {
        if (newWorldPanel.activeSelf)
        {
            if (newWorldName.text.Length > 0 && newWorldName.text.Length < 30) 
            CreateNewWorld();
        }
        else
        {
            OpenNewWorldPanel();
        }
    }
    public void OpenNewWorldPanel()
    {
        newWorldPanel.SetActive(true);
        newWorldButtonText.text = "Подтвердить и создать";
    }


    public void CreateNewWorld()
    {
        SaveData data = GetNewCreatedWorld(newWorldName.text);
        GameObject newWorld = Instantiate(worldToPlayPrefab, content);
        newWorld.GetComponent<WorldSlot>().UpdateDisplay(data);
        saves.Add(data);

        string fullSavePath = savePath + "/" + data.name + ".dat";

        if (!Directory.Exists(savePath))
        {
            Directory.CreateDirectory(savePath);
        }

        BinaryFormatter formatter = new BinaryFormatter();

        using (FileStream fs = new FileStream(fullSavePath, FileMode.OpenOrCreate))
        {
            formatter.Serialize(fs, data);
        }
        newWorldName.text = "";

        TapCancelButton();
    }
    public SaveData GetNewCreatedWorld(string name)
    {
        WorldData world = WorldGenerator.GenerateWorld(70, 70, info);
        SquadData squad = null;
        DateTime currentDateTime = DateTime.Now;
        string formattedTime = currentDateTime.ToString("dd.MM.yyyy HH:mm");
        SaveData data = new SaveData(world, squad, name, formattedTime, "Никогда", 1);

        return data;
    }
}
