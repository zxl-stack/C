import os
from flask import Flask, request, jsonify
from werkzeug.utils import secure_filename
from services.reconstructor import Reconstructor
from services.ai import AIService
from models import db, DigitalArtifact
from config import Config
from utils.storage import upload_to_storage

# 初始化 Flask
app = Flask(__name__)
app.config.from_object(Config)
db.init_app(app)

# 确保上传文件夹存在
if not os.path.exists(app.config['UPLOAD_FOLDER']):
    os.makedirs(app.config['UPLOAD_FOLDER'])

# 初始化服务
reconstructor = Reconstructor()
ai_service = AIService()

@app.route('/api/scan', methods=['POST'])
def handle_scan():
    """处理小程序上传的扫描图片"""
    if 'files' not in request.files:
        return jsonify({"error": "No files uploaded"}), 400
    
    # 保存上传文件
    saved_files = []
    for file in request.files.getlist('files'):
        if file.filename == '':
            continue
        filename = secure_filename(file.filename)
        filepath = os.path.join(app.config['UPLOAD_FOLDER'], filename)
        file.save(filepath)
        saved_files.append(filepath)
    
    if not saved_files:
        return jsonify({"error": "No valid files uploaded"}), 400

    try:
        # 3D重建
        model_id = reconstructor.process(saved_files)
        model_filename = f"{model_id}.glb"
        model_url = upload_to_storage(model_filename, "models")
        
        # AI分析
        artifact_info = ai_service.analyze(saved_files[0])
        
        # 存入数据库
        artifact = DigitalArtifact(
            id=model_id,
            name=artifact_info.get('name', 'Unknown Artifact'),
            model_url=model_url,
            museum_id=request.form.get('museum_id'),
            scan_count=1  # 默认扫描次数为1
        )
        db.session.add(artifact)
        db.session.commit()
        
        return jsonify({
            "model_id": model_id,
            "model_url": model_url,
            "artifact_info": artifact_info
        })
    except Exception as e:
        return jsonify({"error": str(e)}), 500

@app.route('/api/model/<model_id>', methods=['GET'])
def get_model(model_id):
    """获取模型信息"""
    artifact = DigitalArtifact.query.get(model_id)
    if not artifact:
        return jsonify({"error": "Model not found"}), 404
    
    return jsonify({
        "name": artifact.name,
        "model_url": artifact.model_url,
        "scan_count": getattr(artifact, 'scan_count', 0)
    })

if __name__ == '__main__':
    with app.app_context():
        db.create_all()
    app.run(host='0.0.0.0', port=5000)
